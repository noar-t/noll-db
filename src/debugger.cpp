#include <cstdlib>
#include <iostream>

#include <inttypes.h>
#include <fcntl.h>
#include <sys/ptrace.h>
#include <sys/wait.h>

#include "debugger.h"
#include "breakpoint.h"

void debugger::init() {
  init_dwarf_data();
  init_debugee();
}

void debugger::init_dwarf_data() {
  executable_fd = open(executable, O_RDONLY);
  exe_elf = elf::elf(elf::create_mmap_loader(executable_fd));
  exe_dwarf = dwarf::dwarf(dwarf::elf::create_loader(exe_elf));
}

void debugger::init_debugee() {
  pid = fork();

  if (pid == -1) {
    std::perror("Forking errors...");
    exit(EXIT_FAILURE);

  } else if (pid == 0) {
    ptrace(PTRACE_TRACEME, 0, nullptr, nullptr);
    execv(argv[0], &argv[1]);
    std::perror("Exec error");
  }

  wait();
}

void debugger::wait() {
  int wait_status = 0;
  int options = 0;

  waitpid(pid, &wait_status, 0);
}

void debugger::run() {
  continue_debugee();
  return;
}

void debugger::continue_debugee() {
  ptrace(PTRACE_CONT, pid, nullptr, nullptr);
  wait();
}

void debugger::set_breakpoint(std::intptr_t addr) {
  std::cout << "setting breakpoint\n";
  breakpoint b(pid, addr);
  b.enable();
  breakpoint_map.insert({addr, b});
}

void debugger::dump_line_table() {
  for (auto cu : exe_dwarf.compilation_units()) {
    printf("--- <%x>\n", (unsigned int)cu.get_section_offset());
    dwarf::line_table lt = cu.get_line_table();
    for (auto &line : lt) {
      if (line.end_sequence)
        printf("\n");
      else
        printf("%-40s%8d%#20" PRIx64 "\n", line.file->path.c_str(),
            line.line, line.address);
    }
    printf("\n");
  }
}


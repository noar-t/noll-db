#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <utility>

#include <inttypes.h>
#include <fcntl.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <sys/user.h>

#include "debugger.h"
#include "breakpoint.h"

void debugger::init() {
  init_debugee();
  init_dwarf_data();
  init_breakpoints();
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

  waitpid(pid, &wait_status, options);
  std::cerr << "\twait()\n";
}

void debugger::add_src(std::string file_name) {
  src_file_set.insert(file_name);
  std::ifstream file(file_name);
  std::string s;
  for (int line_num = 1; std::getline(file, s); line_num++) {
    std::pair<std::string, int> p(file_name, line_num);
    src_files.insert({p, s});
  }
}

void debugger::run() {
  continue_debugee();
  int i = 10;

  do {
    siginfo_t siginfo = get_siginfo();
    std::cerr << "Waited for signal: " << siginfo.si_signo << "\n";
    step_over_breakpoint();

  } while(i--);
}

void debugger::continue_debugee() {
  ptrace(PTRACE_CONT, pid, nullptr, nullptr);
  wait();
}

void debugger::step_over_breakpoint() {
  uint64_t breakpoint_addr = get_pc() - 1;
  std::cout << "step over:" << std::hex << breakpoint_addr << "\n"; 

  if (breakpoint_map.count(breakpoint_addr)) {
    std::cout << "BREAKPOINT FOUND:" << breakpoint_addr << "\n"; 
    auto b = breakpoint_map[breakpoint_addr];
    b.disable();
    set_pc(breakpoint_addr);
  }
  continue_debugee();
}

void debugger::set_breakpoint(std::intptr_t addr, std::string file,
    unsigned line_num) {
  std::cout << "setting breakpoint at: " << addr << "\n";
  breakpoint b(pid, addr, file, line_num);
  b.enable();
  breakpoint_map.insert({addr, b});
}

void debugger::init_breakpoints() {
  for (dwarf::compilation_unit cu : exe_dwarf.compilation_units()) {
    dwarf::line_table line_table = cu.get_line_table();
    for (dwarf::line_table::entry entry : line_table) {
      if (!entry.end_sequence) {
        std::string file = entry.file->path;
        unsigned line_num = entry.line;
        dwarf::taddr addr = entry.address; 

        set_breakpoint(static_cast<std::intptr_t>(addr), 
                       file,
                       line_num);
      }
    }
  }
}

uint64_t debugger::get_pc() {
  user_regs_struct regs;
  ptrace(PTRACE_GETREGS, pid, nullptr, &regs);
  return static_cast<uint64_t>(regs.rip);
}

void debugger::set_pc(uint64_t rip) {
  user_regs_struct regs;
  ptrace(PTRACE_GETREGS, pid, nullptr, &regs);
  regs.rip = rip;
  ptrace(PTRACE_SETREGS, pid, nullptr, &regs);
}

siginfo_t debugger::get_siginfo() {
  siginfo_t siginfo;
  ptrace(PTRACE_GETSIGINFO, pid, nullptr, &siginfo);
  return siginfo;
}

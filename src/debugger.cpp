#include <cstdlib>
#include <iostream>

#include <fcntl.h>
#include <sys/ptrace.h>
#include <sys/wait.h>

#include "debugger.h"

void debugger::init() {
  init_dwarf_data();
  init_debugee();
}

void debugger::wait() {
  int wait_status = 0;
  int options = 0;

  waitpid(pid, &wait_status, 0);
}

void debugger::run() {
  return;
}

void debugger::continue_debugee() {
  ptrace(PTRACE_CONT, pid, nullptr, nullptr);
  wait();
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

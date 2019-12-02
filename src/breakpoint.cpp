#include <iostream>

#include <sys/ptrace.h>

#include "breakpoint.h"

void breakpoint::enable() {
  if (enabled) // TODO produce an error
    return;

  long inst = ptrace(PTRACE_PEEKDATA, pid, addr, nullptr);
  orig_inst = inst & 0xff;
  const static uint64_t int3 = 0xcc;
  std::cerr << "peek data" << std::hex << inst << "\n";
  inst = ((inst & ~0xff) | int3);
  std::cerr << "poke data" << std::hex << inst << "\n";
  ptrace(PTRACE_POKEDATA, pid, addr, inst);
  long old_inst = ((inst & ~0xff) | orig_inst);
  std::cerr << "unpoke data" << std::hex << old_inst << "\n";
  enabled = true;
}

void breakpoint::disable() {
  if (!enabled) // TODO produce an error
    return;

  long inst = ptrace(PTRACE_PEEKDATA, pid, addr, nullptr);
  long old_inst = ((inst & ~0xff) | orig_inst);
  ptrace(PTRACE_POKEDATA, pid, addr, old_inst);
  enabled = false;
}

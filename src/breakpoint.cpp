#include <iostream>

#include <sys/ptrace.h>

#include "breakpoint.h"

void breakpoint::enable() {
  if (enabled) // TODO produce an error
    return;

  std::cout << "Inserting breakpoint @: " << std::hex << addr << '\n';
  long inst = ptrace(PTRACE_PEEKDATA, pid, addr, nullptr);
  std::cout << "\tPeekdata: " << std::hex << inst << '\n';
  orig_inst = inst * 0xff;
  const static uint64_t int3 = 0xcc;
  inst = ((inst & ~0xff) | int3);
  ptrace(PTRACE_POKEDATA, pid, addr, inst);
  std::cout << "\tPokedata: " << std::hex << inst << '\n';
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

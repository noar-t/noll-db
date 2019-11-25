#include <cstdlib>
#include <iostream>

#include <assert.h>

#include "debugger.h"

int main(int argc, char * argv[]) {
  assert(argc >= 2);
  /* TODO start debugee
     read CFG
     place int3 instructions
     write code to handle breakpoints and move pc
     calculate total breakpoints reached
  */
  // Trim argv
  char ** debugee_argv = &argv[1];
  debugger d(debugee_argv);
  d.init();
  std::intptr_t addr = 0x401be5;
  d.set_breakpoint(addr);
  d.run();
 // d.dump_line_table();
  std::cout << "sleeping\n";
  sleep(3);
  std::cout << "sleep over\n";
  std::cout << "Exiting dbg process\n";
  return EXIT_SUCCESS;
}

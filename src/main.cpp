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
  return EXIT_SUCCESS;
}

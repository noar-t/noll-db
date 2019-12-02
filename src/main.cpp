#include <cstdlib>
#include <iostream>

#include <assert.h>

#include "debugger.h"

int main(int argc, char * argv[]) {
  assert(argc >= 3);
  // argv = ./noll_db <num_src_files> <src_paths> <target_program> <args>

  int num_src_files = atol(argv[1]);
  assert (num_src_files >= 0);

  // Actual program argv
  char ** debugee_argv = &argv[2+num_src_files];

  std::cerr << "Constructing the debugger object\n";
  debugger d(debugee_argv);
  d.init();

  std::cerr << "Caching source files\n";
  for (int i = 0; i < num_src_files; i++) {
    std::string s(argv[i+2]);
    d.add_src(s);
  }

  std::cerr << "Starting debugee\n";
  std::cout << "pc" << d.get_pc() << "\n";
  d.run();
  sleep(3);
  std::cout << "sleeping\n";
  std::cout << "pc" << d.get_pc() << "\n";
  sleep(3);
  std::cout << "sleep over\n";
  std::cout << "Exiting dbg process\n";
  std::cout << "pc" << d.get_pc() << "\n";
  return EXIT_SUCCESS;
}

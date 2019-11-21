#ifndef DEBUGGER_H
#define DEBUGGER_H

#include <string>

#include <unistd.h>

#include "elf/elf++.hh"
#include "dwarf/dwarf++.hh"

class debugger  {
  public:
    debugger(char * argv[]) : 
      argv(argv), executable(argv[0]), pid() {};
    
    void init();
    void run();

  private:
    void init_dwarf_data();
    void init_debugee();
    void wait();
    void continue_debugee();

    char ** argv;
    char * executable;
    int executable_fd;
    pid_t pid;
    elf::elf exe_elf;
    dwarf::dwarf exe_dwarf;
};

#endif

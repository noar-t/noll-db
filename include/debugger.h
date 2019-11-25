#ifndef DEBUGGER_H
#define DEBUGGER_H

#include <string>
#include <cstdint>

#include <unistd.h>

#include "breakpoint.h"
#include "elf/elf++.hh"
#include "dwarf/dwarf++.hh"

class debugger  {
  public:
    debugger(char * argv[]) : 
      argv(argv), executable(argv[0]), pid() {};
    
    void init();
    void run();
    void dump_line_table();
    void set_breakpoint(std::intptr_t addr);

  private:
    void wait();
    void init_debugee();
    void init_dwarf_data();
    void continue_debugee();

    char ** argv;
    char * executable;
    int executable_fd;
    pid_t pid;
    dwarf::dwarf exe_dwarf;
    elf::elf exe_elf;
    std::map<std::intptr_t, breakpoint> breakpoint_map;
};

#endif

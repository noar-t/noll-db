#ifndef DEBUGGER_H
#define DEBUGGER_H

#include <cstdint>
#include <set>
#include <string>

#include <unistd.h>
#include <sys/wait.h>

#include "breakpoint.h"
#include "elf/elf++.hh"
#include "dwarf/dwarf++.hh"

class debugger  {
  public:
    debugger(char * argv[]) : 
      argv(argv), executable(argv[0]), pid() {};
    
    void init();
    void run();
    void add_src(std::string file_name);
    void dump_line_table();
    void set_breakpoint(std::intptr_t addr, std::string file,
                        unsigned line_number);
    uint64_t get_pc();
    void set_pc(uint64_t rip);
    void continue_debugee();
    void step_over_breakpoint();
    siginfo_t get_siginfo();

  private:
    void wait();
    void init_debugee();
    void init_dwarf_data();
    void init_breakpoints();
    //void continue_debugee();
    //uint64_t get_pc();

    char ** argv;
    const char * executable;
    int executable_fd;
    pid_t pid;
    dwarf::dwarf exe_dwarf;
    elf::elf exe_elf;
    std::set<std::string> src_file_set;
    std::map<std::intptr_t, breakpoint> breakpoint_map;
    std::map<std::pair<std::string, int>, std::string> src_files;
};

#endif

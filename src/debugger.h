#ifndef DEBUGGER_H
#define DEBUGGER_H

#include <string>
#include <unistd.h>

#include <elf++.hh>
#include <dwarf++.hh>

class debugger  {
  public:
    debugger(char * argv[]) : 
      argv(argv), executable(argv[0]), pid() {};
    
    void init();
    void run();

  private:
    void wait();
    void init_dwarf_data();
    void continue_debugee();

    char ** argv;
    std::string executable;
    pid_t pid;
    dwarf::dwarf exe_dwarf;
    elf::elf exe_elf;
};

#endif

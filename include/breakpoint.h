#ifndef BREAKPOINT_H
#define BREAKPOINT_H

#include <cstdint>

#include <unistd.h>

class breakpoint {
  public:
    breakpoint(pid_t pid, std::intptr_t addr) :
      pid(pid), addr(addr), enabled(false) {};
    void enable();
    void disable();

  private:
    std::intptr_t addr;
    pid_t pid;
    bool enabled;
    uint8_t orig_inst;
};
#endif

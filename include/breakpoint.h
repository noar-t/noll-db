#ifndef BREAKPOINT_H
#define BREAKPOINT_H

#include <cstdint>
#include <string>

#include <unistd.h>

class breakpoint {
  public:
    breakpoint() {};
    breakpoint(pid_t pid, std::intptr_t addr,
               std::string containing_file, unsigned line_number) :
      pid(pid), addr(addr), containing_file(containing_file),
      line_number(line_number), enabled(false), hit(false) {};
    void enable();
    void disable();

  private:
    std::intptr_t addr;
    pid_t pid;
    bool enabled;
    uint8_t orig_inst;

    bool hit;
    std::string containing_file;
    unsigned line_number;
};
#endif

#ifndef IO_h
#define IO_h
#include "Module.h"

class IO : public Module {
  public:
    IO();
    virtual float process(int functionID, int polyChannel);
    enum {MAIN_OUTPUT_IN, MAIN_INPUT_OUT};

  private:
};

#endif
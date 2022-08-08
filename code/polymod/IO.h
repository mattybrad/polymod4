#ifndef IO_h
#define IO_h
#include "Module.h"

class IO : public Module {
  public:
    IO();
    virtual float process(int functionID);
    float *mainIn = nullptr;

  private:
};

#endif
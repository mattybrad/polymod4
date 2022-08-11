#ifndef IO_h
#define IO_h
#include "Module.h"

class IO : public Module {
  public:
    IO();
    //virtual void prepare();
    virtual void process();
    float getOutput();
  private:
};

#endif
#ifndef IO_h
#define IO_h
#include "Module.h"

class IO : public Module {
  public:
    IO();
    virtual float process(int functionID);
    float *mainIn = nullptr;
    static const int MAIN_IN = 0;

  private:
};

#endif
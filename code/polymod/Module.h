#ifndef Module_h
#define Module_h
#include <stdint.h>

class Module {
  public:
    Module();
    void addPseudoConnection(uint8_t sourceSocketNum, uint8_t destSocketNum);
    virtual float process(int functionID);

  protected:
     
  private:
    
    
};

#endif
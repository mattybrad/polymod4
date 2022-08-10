#ifndef Module_h
#define Module_h
#include <stdint.h>

class Socket;

class Module {
  public:
    Module();
    Socket *tempSocket = nullptr;
    void addPseudoConnection(uint8_t sourceSocketNum, uint8_t destSocketNum);
    float *inputFloats[8];
    virtual float process(int functionID);

  protected:
     
  private:
    
    
};

#endif
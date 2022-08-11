#ifndef Module_h
#define Module_h
#include <stdint.h>

class Socket;

class Module {
  public:
    Module();
    virtual void init();
    void addPseudoConnection(int sourceParam, int destParam);
    int pseudoSources[8]; // pseudoSources[destination] = source
    Socket *sockets[8]; // pointers to sockets
    float *inputFloats[8];
    virtual float process(int functionID);

  protected:
     
  private:
    
    
};

#endif
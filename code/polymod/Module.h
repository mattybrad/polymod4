#ifndef Module_h
#define Module_h
#include <stdint.h>

class Socket;

class Module {
  public:
    Module();
    void addPseudoConnection(int sourceParam, int destParam);
    int pseudoSources[16][2]; // pseudoSources[source][dest]
    int numPseudoSources = 0;
    Socket *sockets[8]; // pointers to sockets
    virtual float process(int functionID);
    static const int MAX_POLYPHONY = 4;

  protected:
    float _sampleRate = 48000.0f;
     
  private:
    
};

#endif
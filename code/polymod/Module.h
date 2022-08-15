#ifndef Module_h
#define Module_h
#include <stdint.h>

class Socket;

class Module {
  public:
    Module();
    void addPseudoConnection(int sourceParam, int destParam);
    int numPseudoSources = 0;
    virtual float process(int functionID, int polyChannel);
    static const int MAX_POLYPHONY = 4;
    static const int MAX_MODULE_SOCKETS = 8;
    int pseudoSources[MAX_MODULE_SOCKETS][2]; // pseudoSources[source][dest]
    Socket *sockets[MAX_MODULE_SOCKETS]; // pointers to sockets
    float inputFloats[MAX_MODULE_SOCKETS][MAX_POLYPHONY];

  protected:
    float _sampleRate = 48000.0f;
     
  private:
    
};

#endif
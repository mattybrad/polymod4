#ifndef SocketConnection_h
#define SocketConnection_h
#include "Arduino.h"
#include "Globals.h"
#include "SocketInput.h"
#include "SocketOutput.h"
#include <Audio.h>
 
class SocketConnection {
  public:
    SocketConnection();
    void connect(unsigned int outNum, SocketOutput& src, unsigned int inNum, SocketInput& dest, int connNum);
    void disconnect();

    // following vars should really be private, with getter functions
    bool inUse = false;
    unsigned int outputSocketNum = 999;
    unsigned int inputSocketNum = 999;
    unsigned int checkNum = 0;
    bool poly = false;
    bool confirmed = false;
    SocketOutput *_src = NULL;
    SocketInput *_dest = NULL;
  private:
    AudioConnection* _patchCables[MAX_POLYPHONY];
    
};

#endif

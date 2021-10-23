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
    void connect(SocketOutput& src, SocketInput& dest);
    void disconnect();
    bool inUse = false;
  private:
    AudioConnection* _patchCables[MAX_POLYPHONY];
    
};

#endif

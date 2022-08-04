#ifndef Module_h
#define Module_h
#include <stdint.h>
#include "Socket.h"

class Module {
  public:
    Module();
    //void mapInput(uint8_t physicalInputNum, uint8_t virtualInputNum);
    //void mapOutput(uint8_t physicalOutputNum, uint8_t virtualOutputNum);
    //void mapPot(uint8_t physicalPotNum, uint8_t virtualPotNum);
    virtual void prepare();
    virtual void process();
    Socket _sockets[8]; // protected eventually...?

  protected:
     
  private:
    //void _setPolyLink(uint8_t virtualOutputNum, uint8_t virtualInputNum);
    
};

#endif
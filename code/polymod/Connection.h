#ifndef Connection_h
#define Connection_h
#include <stdint.h>

class Connection {
  public:
    Connection();
    uint8_t physicalOutputNum;
    uint8_t physicalInputNum;
    bool isConnected = false;

  private:
    
};

#endif
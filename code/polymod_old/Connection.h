#ifndef Connection_h
#define Connection_h
#include <stdint.h>

class Connection {
  public:
    Connection();
    bool isConnected();
    void process();
    uint8_t physicalOutputNum;
    uint8_t physicalInputNum;
    bool _isConnected = false;

  private:
    
};

#endif
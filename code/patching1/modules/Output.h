#ifndef Output_h
#define Output_h
#include "Module.h"

class Output : public Module {
  public:
    Output();
    virtual void update();
    //SocketOutput audioOut;
    //SocketInput freqModIn;
  private:
    //AnalogControl _freqControl;
    //AudioSynthWaveformModulated _osc[MAX_POLYPHONY];
    //AudioMixer4 _modMixer[MAX_POLYPHONY];
};

#endif
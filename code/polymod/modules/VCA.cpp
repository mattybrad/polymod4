#include "VCA.h"

VCA::VCA()
{
    addPseudoConnection(AUDIO_IN, AUDIO_OUT);
    addPseudoConnection(CONTROL_IN, AUDIO_OUT);
}

float VCA::process(int functionID, int polyChannel, int sampleNum)
{
    float returnVal = 0.0f;
    switch (functionID)
    {
    case AUDIO_OUT:
        float newGain = inputFloats[CONTROL_IN][polyChannel];
        if(newGain < 0) newGain = 0;
        returnVal = inputFloats[AUDIO_IN][polyChannel] * newGain;
        break;
    }
    return returnVal;
}
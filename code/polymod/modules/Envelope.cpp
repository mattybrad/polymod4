#include "Envelope.h"

Envelope::Envelope()
{
    for (int i = 0; i < MAX_POLYPHONY; i++)
    {
        adsr[i].Init(_sampleRate);
        adsr[i].SetTime(ADSR_SEG_ATTACK, .001);
        adsr[i].SetTime(ADSR_SEG_DECAY, .05);
        adsr[i].SetTime(ADSR_SEG_RELEASE, 3.0);
        adsr[i].SetSustainLevel(.3);
    }
    addPseudoConnection(GATE_IN, CONTROL_OUT);
}

float Envelope::process(int functionID, int polyChannel, int sampleNum)
{
    float returnVal = 0.0f;
    switch (functionID)
    {
    case CONTROL_OUT:
        if(sampleNum == 0) {
            adsr[polyChannel].SetTime(ADSR_SEG_ATTACK, attackControl);
            adsr[polyChannel].SetTime(ADSR_SEG_DECAY, decayControl);
            adsr[polyChannel].SetSustainLevel(sustainControl);
            adsr[polyChannel].SetTime(ADSR_SEG_RELEASE, 3 * releaseControl);
        }
        bool gate = inputFloats[GATE_IN][polyChannel] > 0.1;
        returnVal = adsr[polyChannel].Process(gate);
        break;
    }
    return returnVal;
}
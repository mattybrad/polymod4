#include "VCF.h"

VCF::VCF()
{
    for (int i = 0; i < MAX_POLYPHONY; i++)
    {
        filter[i].Init(_sampleRate);
        filter[i].SetRes(0.5);
        filter[i].SetFreq(500.0);
    }
    addPseudoConnection(AUDIO_IN, LPF_OUT);
    addPseudoConnection(FREQ_IN, LPF_OUT);
}

float VCF::process(int functionID, int polyChannel, int sampleNum)
{
    float returnVal = 0.0f;
    switch (functionID)
    {
    case LPF_OUT:
        if (sampleNum == 0)
        {
            //float newFreq = 2000.0f + 1950.0f * inputFloats[FREQ_IN][polyChannel];
            filter[polyChannel].SetRes(resControl);
            float newFreq = 500.0f * pow(2.0, 10.0 * (inputFloats[FREQ_IN][polyChannel] + cutoffControl - 0.5));
            if(newFreq < 0.1) newFreq = 0.1;
            else if(newFreq > 25000) newFreq = 25000; // not sure exactly what max freq is, or why there is a max
            filter[polyChannel].SetFreq(newFreq);
        }
        returnVal = filter[polyChannel].Process(inputFloats[AUDIO_IN][polyChannel]);
        break;
    }
    return returnVal;
}
#include "BitCrusher.h"

// Module setup:
// SOCKETS
// 0 (in) Audio in
// 2 (in) Freq mod in
// 3 (out) Low pass filter out

BitCrusher::BitCrusher()
{
    for (int i = 0; i < MAX_POLYPHONY; i++)
    {
        bc[i].Init(_sampleRate);
        bc[i].SetBitDepth(2);
        bc[i].SetCrushRate(4000.0f);
    }
    addPseudoConnection(AUDIO_IN, AUDIO_OUT);
}

float BitCrusher::process(int functionID, int polyChannel, int sampleNum)
{
    float returnVal = 0.0f;
    switch (functionID)
    {
    case AUDIO_OUT:
        returnVal = bc[polyChannel].Process(inputFloats[AUDIO_IN][polyChannel]);
        break;
    }
    return returnVal;
}
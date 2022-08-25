#include "Mixer.h"

Mixer::Mixer()
{
    addPseudoConnection(INPUT_1, OUTPUT);
    addPseudoConnection(INPUT_2, OUTPUT);
}

float Mixer::process(int functionID, int polyChannel, int sampleNum)
{
    float returnVal = 0.0f;
    switch (functionID)
    {
    case OUTPUT:
        returnVal = inputFloats[INPUT_1][polyChannel] + inputFloats[INPUT_2][polyChannel];
        break;
    }
    return returnVal;
}
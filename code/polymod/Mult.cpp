#include "Mult.h"

Mult::Mult()
{
    addPseudoConnection(INPUT, OUTPUT_1);
    addPseudoConnection(INPUT, OUTPUT_2);
}

float Mult::process(int functionID, int polyChannel, int sampleNum)
{
    float returnVal = 0.0f;
    // a mult is so simple that I could actually not have a switch, but whatever
    switch (functionID)
    {
    case OUTPUT_1:
        returnVal = inputFloats[INPUT][polyChannel];
        break;
    case OUTPUT_2:
        returnVal = inputFloats[INPUT][polyChannel];
        break;
    }
    return returnVal;
}
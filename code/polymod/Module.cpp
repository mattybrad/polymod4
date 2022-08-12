#include "Module.h"

Module::Module()
{
    for (int i = 0; i < 8; i++)
    {
        for(int j=0; j<MAX_POLYPHONY; j++) {
            inputFloats[i][j] = nullptr;
            pseudoSources[i][0] = -1;
            pseudoSources[i][1] = -1;
        }
    }
}

void Module::addPseudoConnection(int source, int dest)
{
    if(numPseudoSources < 16) {
        pseudoSources[numPseudoSources][0] = source;
        pseudoSources[numPseudoSources][1] = dest;
        numPseudoSources ++;
    }
}

float Module::process(int functionID, int polyChannel)
{
    return 0.0f;
}
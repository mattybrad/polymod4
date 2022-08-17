#include "Module.h"

Module::Module()
{
    for (int i = 0; i < MAX_MODULE_SOCKETS; i++)
    {
        pseudoSources[i][0] = -1;
        pseudoSources[i][1] = -1;
    }
}

void Module::addPseudoConnection(int source, int dest)
{
    if (numPseudoSources < MAX_MODULE_SOCKETS)
    {
        pseudoSources[numPseudoSources][0] = source;
        pseudoSources[numPseudoSources][1] = dest;
        numPseudoSources++;
    }
}

float Module::process(int functionID, int polyChannel)
{
    return 0.0f;
}
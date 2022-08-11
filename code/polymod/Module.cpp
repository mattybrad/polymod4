#include "Module.h"

Module::Module()
{
    for (int i = 0; i < 8; i++)
    {
        inputFloats[i] = nullptr;
        pseudoSources[i] = -1;
    }
}

void Module::addPseudoConnection(int source, int dest)
{
    pseudoSources[dest] = source;
}

float Module::process(int functionID)
{
    return 0.0f;
}
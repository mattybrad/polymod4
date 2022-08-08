#include <iostream>

#include "Module.h"
#include "Socket.h"

Socket socket;
Module module1;
Module module2;

float testFunc(float inVal) {
    return inVal * 5.0f;
}

int main()
{
    int testInt = 5;
    int testFloat = 1.2345;
    std::cout << "Learning about pointers..." << std::endl;

    module1.freq = 2.0f;
    module2.freq = 3.0f;
    
    socket.module = &module1;
    socket.moduleFunction = &Module::processA;
    socket.process();
    std::cout << socket.outVal << std::endl;

    socket.module = &module2;
    socket.moduleFunction = &module2.processA;
    socket.process();
    std::cout << socket.outVal << std::endl;
}
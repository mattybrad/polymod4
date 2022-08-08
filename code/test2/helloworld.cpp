#include <iostream>

#include "Module.h"
#include "Socket.h"

Socket socket;
Module module;

int main()
{
    int testInt = 5;
    int testFloat = 1.2345;
    std::cout << "Learning about pointers..." << std::endl;
    std::cout << testInt << std::endl;
    std::cout << testFloat << std::endl;

    socket.module = &module;
    socket.process();
    std::cout << socket.outVal << std::endl;
    socket.inVal = socket.outVal;
    socket.process();
    std::cout << socket.outVal << std::endl;
    socket.inVal = socket.outVal;
    socket.process();
    std::cout << socket.outVal << std::endl;
    socket.inVal = socket.outVal;
    socket.process();
    std::cout << socket.outVal << std::endl;
    socket.inVal = socket.outVal;
}
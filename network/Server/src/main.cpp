#include "Server.hpp"
#include <iostream>

int main()
{
    Server server;

    try
    {
        server.start();
    }
    catch(...)
    {
        std::cerr << "Failed to run server" << std::endl;
        return -1;
    }

    return 0;
}
#include "Server/Server.hpp"
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#else
#include <csignal>
#endif

// So fucked up....
static Server server;

#ifdef _WIN32
BOOL WINAPI WinHandler(DWORD ctrlType)
{
    if (ctrlType == CTRL_C_EVENT)
    {
        std::cout << "\nCaught CTRL+C on Windows.\n";
        server.stop();
        return TRUE;
    }

    return FALSE;
}
#else
void signalHandler(int signal)
{
    if (signal == SIGINT)
    {
        std::cout << "\nCaught CTRL+C. Shutting down server...\n";
        server.stop();
    }
}

#endif

int main()
{
#ifdef _WIN32
    SetConsoleCtrlHandler(WinHandler, TRUE);
#else
    std::signal(SIGINT, signalHandler);
#endif

    try
    {
        server.start();
    }
    catch (...)
    {
        std::cerr << "Failed to run server" << std::endl;
        return -1;
    }

    return 0;
}
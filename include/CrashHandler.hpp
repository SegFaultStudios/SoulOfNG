#ifndef CRASH_HANDLER_HPP
#define CRASH_HANDLER_HPP

#ifdef _WIN32
    #include <windows.h>
    #include <dbghelp.h>
#else 
    #include <execinfo.h>
    #include <signal.h>
    #include <unistd.h>
    #include <sstream>
    #include <iostream>
    #include <cstdio>
    #include <cxxabi.h>
#endif 

#include <fstream>

//TODO Does not work for linux
class CrashHandler
{
public:
    static void setupCrashHandler();

private:
#ifdef _WIN32
    static LONG WINAPI crashHandler(EXCEPTION_POINTERS* ExceptionInfo);
#else
    static void crashHandler(int signum);
#endif
};

#endif //CRASH_HANDLER_HPP
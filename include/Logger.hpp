#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <iostream>
#include <string>
#include <mutex>
#include <cstdint>
#include <fstream>
#include <memory>

#ifdef _WIN32
    #include <windows.h>
#endif 

//TODO remake its

namespace terminalColors
{
#ifdef _WIN32
    using TerminalColorType = int;

    const TerminalColorType RED = FOREGROUND_RED | FOREGROUND_INTENSITY;
    const TerminalColorType GREEN = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
    const TerminalColorType YELLOW = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
    const TerminalColorType WHITE = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
    const TerminalColorType BLUE = FOREGROUND_BLUE | FOREGROUND_INTENSITY;

    inline void logColoredMessageToTerminal(TerminalColorType color, const std::string& message)
    {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, color);

        std::cout << message << '\n';
    }
#else
    using TerminalColorType = std::string;

    const TerminalColorType RED = "\033[31m";
    const TerminalColorType GREEN = "\033[32m";
    const TerminalColorType YELLOW = "\033[33m";
    const TerminalColorType WHITE = "\033[37m";
    const TerminalColorType BLUE = "\033[34m";

    inline void logColoredMessageToTerminal(const TerminalColorType& color, const std::string& message)
    {
        std::cout << color << message << "\033[0m" << '\n';
    }
#endif
} // namespace terminalColors

class Logger
{
public:
    enum class LogLevel : uint8_t
    {
        DEBUG = 0,
        INFO = 1,
        WARNING = 2,
        LOG_LEVEL_ERROR = 3,
    };

    Logger();

    explicit Logger(const std::string& logFilePath);

    void debug(const std::string& message);
    void info(const std::string& message);
    void error(const std::string& message);
    void warning(const std::string& message);

    void log(LogLevel logLevel, const std::string& message);

    static void setDefaultLogger(std::unique_ptr<Logger> logger);
    static void createDefaultLogger();
    static Logger* getDefaultLogger();

    ~Logger();
private:
    static std::string getCurrentTime();
    static std::string logLevelToString(LogLevel level);
    static terminalColors::TerminalColorType logLevelToColor(LogLevel level);

    std::mutex m_logMutex;

    bool m_isConsoleOutput{true};
    bool m_isOutputFile{false};

    std::ofstream m_logFile;

    static inline std::unique_ptr<Logger> s_defaultLogger{nullptr};
};

#define LOG_DEBUG(message) Logger::getDefaultLogger()->debug(message)
#define LOG_INFO(message) Logger::getDefaultLogger()->info(message)
#define LOG_WARNING(message) Logger::getDefaultLogger()->warning(message)
#define LOG_ERROR(message) Logger::getDefaultLogger()->error(message)

#endif //LOGGER_HPP
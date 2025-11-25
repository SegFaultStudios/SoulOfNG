#include "Logger.hpp"
#include <chrono>
#include <sstream>
#include <iomanip>

Logger::Logger() = default;

Logger::Logger(const std::string& logFilePath)
{
    if(!logFilePath.empty())
    {
        m_logFile.open(logFilePath, std::ios::app);
        m_isOutputFile = m_logFile.is_open();
    }
}

void Logger::setDefaultLogger(std::unique_ptr<Logger> logger)
{
    s_defaultLogger = std::move(logger);
}

void Logger::createDefaultLogger()
{
    s_defaultLogger = std::make_unique<Logger>();
}

Logger* Logger::getDefaultLogger()
{
    return s_defaultLogger.get();
}

void Logger::debug(const std::string& message)
{
#if DEBUG_MODE
    log(LogLevel::DEBUG, message);
#endif 
}

void Logger::info(const std::string& message)
{
    log(LogLevel::INFO, message);
}

void Logger::error(const std::string& message)
{
    log(LogLevel::ERROR, message);
}

void Logger::warning(const std::string& message)
{
    log(LogLevel::WARNING, message);
}

void Logger::log(LogLevel logLevel, const std::string& message)
{
    std::lock_guard<std::mutex> lock(m_logMutex);
    
    std::string timestamp = getCurrentTime();
    std::string levelStr = logLevelToString(logLevel);

    terminalColors::TerminalColorType color = logLevelToColor(logLevel);
    
    std::stringstream logEntry;

    logEntry << "[" << timestamp << "] "
                << "[" << levelStr << "] "
                << message;

    if(m_isConsoleOutput)
        terminalColors::logColoredMessageToTerminal(color, logEntry.str());
    
    if(m_isOutputFile)
        m_logFile << logEntry.str() << '\n';
}

std::string Logger::getCurrentTime()
{
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()) % 1000;
    
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
    ss << "." << std::setfill('0') << std::setw(3) << ms.count();
    return ss.str();
}

terminalColors::TerminalColorType Logger::logLevelToColor(LogLevel level)
{
    switch (level)
    {
        case LogLevel::DEBUG: return terminalColors::BLUE;
        case LogLevel::INFO: return terminalColors::GREEN;
        case LogLevel::WARNING: return terminalColors::YELLOW;
        case LogLevel::ERROR: return terminalColors::RED;
        default: return terminalColors::WHITE;
    }
}

std::string Logger::logLevelToString(LogLevel level)
{
    switch (level)
    {
        case LogLevel::DEBUG: return "DEBUG";
        case LogLevel::INFO: return "INFO";
        case LogLevel::WARNING: return "WARNING";
        case LogLevel::ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

Logger::~Logger()
{
    if(m_logFile.is_open())
        m_logFile.close();
}

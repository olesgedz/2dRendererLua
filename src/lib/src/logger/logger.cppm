module;

#include <chrono>
#include <iostream>
#include <ctime>
#include <map>

export module logger;

std::string currentDateTimeToString();


export class Logger {
public:
  enum class LogType {
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR
  };

  enum class LogColor {
    RED,
    GREEN,
    YELLOW,
    BLUE,
    WHITE,
    CYAN,
    MAGENTA,
    BLACK
  };

  static std::map<LogColor, std::string> logColorMap;

  struct LogEntry {
    LogType type;
    std::string message;
    // std::string timestamp;
  };

  static std::vector<LogEntry> messages;

  static void log(const std::string& message, LogColor color);
  static void err(const std::string& message);
};

std::vector<Logger::LogEntry> Logger::messages;

void Logger::err(const std::string& message) {
  LogEntry logEntry;

  logEntry.type = LogType::LOG_ERROR;
  logEntry.message = "ERR: [" + currentDateTimeToString() + "]: " + message;

  messages.push_back(logEntry);

  std::cerr << "\x1B[91m" << logEntry.message << "\033[0m" << std::endl;
}

void Logger::log(const std::string& message, LogColor color = LogColor::GREEN) {
  LogEntry logEntry;

  logEntry.type = LogType::LOG_INFO;
  logEntry.message = "LOG: [" + currentDateTimeToString() + "]: " + message;

  messages.push_back(logEntry);

  std::cout << logColorMap[color] << logEntry.message << "\033[0m" << std::endl;
}

std::string currentDateTimeToString() {
  std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  std::string output(30, '\0');
  std::strftime(&output[0], output.size(), "%Y-%m-%d %H:%M:%S", std::localtime(&now));

  return output;
}

std::map<Logger::LogColor, std::string> Logger::logColorMap = {
    {LogColor::RED, "\x1B[91m"},
    {LogColor::GREEN, "\x1B[92m"},
    {LogColor::YELLOW, "\x1B[93m"},
    {LogColor::BLUE, "\x1B[94m"},
    {LogColor::WHITE, "\x1B[97m"},
    {LogColor::CYAN, "\x1B[96m"},
    {LogColor::MAGENTA, "\x1B[95m"},
    {LogColor::BLACK, "\x1B[30m"}
};
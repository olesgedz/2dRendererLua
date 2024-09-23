module;

#include <chrono>
#include <iostream>
#include <ctime>

export module Logger;

std::string currentDateTimeToString();


export class Logger {
public:
  enum class LogType {
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR
  };

  struct LogEntry {
    LogType type;
    std::string message;
    // std::string timestamp;
  };

  static std::vector<LogEntry> messages;

  static void log(const std::string& message);
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

void Logger::log(const std::string& message) {
  LogEntry logEntry;

  logEntry.type = LogType::LOG_INFO;
  logEntry.message = "LOG: [" + currentDateTimeToString() + "]: " + message;

  messages.push_back(logEntry);

  std::cout << "\x1B[32m" << message << "\033[0m" << std::endl;
}

std::string currentDateTimeToString() {
  std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  std::string output(30, '\0');
  std::strftime(&output[0], output.size(), "%Y-%m-%d %H:%M:%S", std::localtime(&now));

  return output;
}
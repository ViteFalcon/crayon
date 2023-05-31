#pragma once
#include <fmt/core.h>

#include <string>

namespace crayon {
namespace log {

enum class LogLevel {
  Trace,
  Debug,
  Info,
  Warning,
  Error,
  Critical,
};

template <typename T>
void log(LogLevel level, const T& msg);

template <>
void log(LogLevel level, const std::string& message);

template <typename T>
void log(LogLevel level, const T& msg) {
  const std::string result = fmt::format(msg);
  log(level, result);
}

template <typename... Args>
void log(LogLevel level, fmt::format_string<Args...> format, Args&&... args) {
  std::string result = fmt::format(format, std::forward<Args>(args)...);
  log(level, result);
}

void initialize();

template <typename T>
inline void trace(const T& msg) {
  log(LogLevel::Trace, msg);
}

template <typename... Args>
inline void trace(fmt::format_string<Args...> format, Args&&... args) {
  log(LogLevel::Trace, format, std::forward<Args>(args)...);
}

template <typename T>
inline void debug(const T& msg) {
  log(LogLevel::Debug, msg);
}

template <typename... Args>
inline void debug(fmt::format_string<Args...> format, Args&&... args) {
  log(LogLevel::Debug, format, std::forward<Args>(args)...);
}

template <typename T>
inline void info(const T& msg) {
  log(LogLevel::Info, msg);
}

template <typename... Args>
inline void info(fmt::format_string<Args...> format, Args&&... args) {
  log(LogLevel::Info, format, std::forward<Args>(args)...);
}

template <typename T>
inline void warn(const T& msg) {
  log(LogLevel::Warning, msg);
}

template <typename... Args>
inline void warn(fmt::format_string<Args...> format, Args&&... args) {
  log(LogLevel::Warning, format, std::forward<Args>(args)...);
}

template <typename T>
inline void error(const T& msg) {
  log(LogLevel::Error, msg);
}

template <typename... Args>
inline void error(fmt::format_string<Args...> format, Args&&... args) {
  log(LogLevel::Error, format, std::forward<Args>(args)...);
}

template <typename T>
inline void critical(const T& msg) {
  log(LogLevel::Critical, msg);
}

template <typename... Args>
inline void critical(fmt::format_string<Args...> format, Args&&... args) {
  log(LogLevel::Critical, format, std::forward<Args>(args)...);
}

}  // namespace log
}  // namespace crayon
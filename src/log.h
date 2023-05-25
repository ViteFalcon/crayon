#pragma once
#include <spdlog/spdlog.h>

namespace crayon {
#define log_trace spdlog::trace
#define log_debug spdlog::debug
#define log_info spdlog::info
#define log_error spdlog::error
#define log_critical spdlog::critical

namespace log {
template <typename T>
inline void trace(const T& msg) {
  spdlog::trace(msg);
}

template <typename... Args>
inline void trace(spdlog::format_string_t<Args...> format, Args&&... args) {
  spdlog::trace(format, std::forward<Args>(args)...);
}

template <typename T>
inline void debug(const T& msg) {
  spdlog::debug(msg);
}

template <typename... Args>
inline void debug(spdlog::format_string_t<Args...> format, Args&&... args) {
  spdlog::debug(format, std::forward<Args>(args)...);
}

template <typename T>
inline void info(const T& msg) {
  spdlog::trace(msg);
}

template <typename... Args>
inline void info(spdlog::format_string_t<Args...> format, Args&&... args) {
  spdlog::info(format, std::forward<Args>(args)...);
}

template <typename T>
inline void error(const T& msg) {
  spdlog::trace(msg);
}

template <typename... Args>
inline void error(spdlog::format_string_t<Args...> format, Args&&... args) {
  spdlog::error(format, std::forward<Args>(args)...);
}

template <typename T>
inline void critical(const T& msg) {
  spdlog::trace(msg);
}

template <typename... Args>
inline void critical(spdlog::format_string_t<Args...> format, Args&&... args) {
  spdlog::critical(format, std::forward<Args>(args)...);
}
}  // namespace log
}  // namespace crayon
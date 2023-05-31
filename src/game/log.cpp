#include "log.h"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include <memory>

namespace crayon {
namespace log {

static std::shared_ptr<::spdlog::logger> console;
static std::shared_ptr<::spdlog::logger> err_logger;

void initialize() {
  if (console || err_logger) {
    return;
  }
  ::spdlog::set_level(::spdlog::level::debug);
  console = ::spdlog::stdout_color_mt("console");
  err_logger = ::spdlog::stderr_color_mt("stderr");
}

template <>
void log(LogLevel level, const ::std::string& message) {
  switch (level) {
    case LogLevel::Trace:
      ::spdlog::trace(message);
      break;
    case LogLevel::Debug:
      ::spdlog::debug(message);
      break;
    case LogLevel::Info:
      ::spdlog::info(message);
      break;
    case LogLevel::Warning:
      ::spdlog::warn(message);
      break;
    case LogLevel::Error:
      ::spdlog::error(message);
      break;
    case LogLevel::Critical:
    default:
      ::spdlog::critical(message);
      break;
  }
}

}  // namespace log
}  // namespace crayon
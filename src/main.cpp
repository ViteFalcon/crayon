#include <raylib.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <codecvt>
#include <iostream>

#include "core/string.h"
#include "log.h"

namespace crayon {
/// <summary>
/// This class is used to make use of RAII to cleanly exit the game
/// </summary>
class GameRunner {
 public:
  GameRunner() {
    std::setlocale(LC_ALL, "en_US.utf16");
    // TODO: Select the monitor based on settings
    // GetMonitorCount() can be used to test if the monitor setting should be honored
    int current_monitor = ::GetCurrentMonitor();
    int width = ::GetMonitorWidth(current_monitor);
    int height = ::GetMonitorHeight(current_monitor);
    ::InitWindow(width, height, "Crayon - rAthena Client");
  }

  ~GameRunner() { ::CloseWindow(); }

  void run() {
    String my_str = u"很有用";
    log::debug(my_str.length());
    std::wstring_convert<std::codecvt_utf8<char16_t>, char16_t> converter;
    log::debug(converter.to_bytes(my_str));
    StringBytes bytes = to_bytes(my_str);
    log::debug("Total bytes: {}", bytes.data.size());
    while (!WindowShouldClose()) {
      run_frame(GetTime());
    }
  }

  void run_frame(double delta_time_sec) {
    ::BeginDrawing();
    ::ClearBackground(RAYWHITE);
    ::EndDrawing();
  }
};
}  // namespace crayon

void raylib_log(int log_level, const char* text, va_list args) {
  using namespace crayon::log;
  static char buffer[1024];
  sprintf_s(buffer, 1024, text, args);
  switch (log_level) {
    case LOG_TRACE:
      trace(buffer);
      break;
    case LOG_DEBUG:
      debug(buffer);
      break;
    case LOG_INFO:
      info(buffer);
      break;
    case LOG_WARNING:
      spdlog::warn(buffer);
      break;
    case LOG_ERROR:
      error(buffer);
      break;
    case LOG_FATAL:
    default:
      critical(buffer);
      break;
  }
}

int main() {
  crayon::GameRunner game_runner;
  try {
    spdlog::set_level(spdlog::level::debug);
    SetTraceLogCallback(raylib_log);
    auto console = spdlog::stdout_color_mt("console");
    auto err_logger = spdlog::stderr_color_mt("stderr");
    game_runner.run();
  } catch (const std::exception& ex) {
    crayon::log::critical("Unexpected error. Reason: {}", ex.what());
  }
  return 0;
}

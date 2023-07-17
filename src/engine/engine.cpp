#define _WINUSER_
#define _WINGDI_
#define _IMM_
#define _APISETCONSOLEL3_
#include "engine.h"

#include <consoleapi3.h>
#include <engine/audio/audio_device.h>
#include <game/log.h>
#include <imm.h>
#include <raygui.h>
#include <raylib.h>

#include <filesystem>

namespace crayon {

static void raylib_log(int log_level, const char* text, va_list args) {
  using namespace crayon;
  static constexpr int BUFFER_SIZE = 1024;
  char buffer[BUFFER_SIZE] = {0};
  vsprintf_s(buffer, BUFFER_SIZE, text, args);
  std::string message(buffer);
  switch (log_level) {
    case LOG_TRACE:
      log::trace(message);
      break;
    case LOG_DEBUG:
      log::debug(message);
      break;
    case LOG_INFO:
      log::info(message);
      break;
    case LOG_WARNING:
      log::warn(message);
      break;
    case LOG_ERROR:
      log::error(message);
      break;
    case LOG_FATAL:
    default:
      log::critical(message);
      break;
  }
}

Engine::Engine(const GameConfig& config, EngineResourceLoader& resource_loader)
    : config_(config), resource_loader_(resource_loader) {
  std::setlocale(LC_ALL, "en_US.utf16");
  ::SetTraceLogCallback(raylib_log);
  ::SetTraceLogLevel(LOG_DEBUG);
  log::initialize();
  // TODO: Select the monitor based on settings
  // GetMonitorCount() can be used to test if the monitor setting should be honored
  int current_monitor = ::GetCurrentMonitor();
  int width = ::GetMonitorWidth(current_monitor);
  int height = ::GetMonitorHeight(current_monitor);
  InitWindow(width, height, config_.window_title().c_str());
  audio_device_ = std::make_unique<AudioDevice>(resource_loader_);
}

Engine::~Engine() {
  audio_device_.reset();
  CloseWindow();
}

void Engine::run(GameUpdater updater) {
  audio_device_->set_bgm(config_.title_bgm_path().string());
  while (!WindowShouldClose()) {
    run_frame(GetTime(), updater);
  }
}

void Engine::run_frame(double delta_time_secs, GameUpdater updater) {
  audio_device_->update();
  ::BeginDrawing();
  ::ClearBackground(RAYWHITE);
  updater(delta_time_secs);
  ::EndDrawing();
}

void Engine::show_auth() {}

void Engine::notify_progress(const String& title_id) {}

void Engine::notify_error(const String& message_id, const String& title_id) {}

void Engine::show_server_selection(const std::vector<CharacterServer> servers) {}

}  // namespace crayon

#undef _APISETCONSOLEL3_
#undef _WINUSER_
#undef _WINGDI_
#undef _IMM_

#pragma once
#include <functional>
#include <memory>

#include "../game/core/string.h"
#include "../game/engine_interface.h"
#include "../game/game_config.h"

namespace crayon {
class AudioDevice;

using GameUpdater = std::function<void(double)>;

class Engine : public EngineInterface {
 public:
  Engine(const GameConfig& config);
  ~Engine();

  void run(GameUpdater updater);

 private:
  void run_frame(double delta_time_secs, GameUpdater updater);

  void show_auth() override;

  void notify_progress(const String& title_id) override;

  void notify_error(const String& message_id, const String& title_id = EMPTY_STRING) override;

  void show_server_selection(const std::vector<CharacterServer> servers) override;

 private:
  const GameConfig _config;
  std::unique_ptr<AudioDevice> _audio_device;
};
}  // namespace crayon
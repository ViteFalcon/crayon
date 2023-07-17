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
  Engine(const GameConfig& config, EngineResourceLoader& resource_loader);
  ~Engine();

  void run(GameUpdater updater);

 private:
  void run_frame(double delta_time_secs, GameUpdater updater);

  void show_auth() override;

  void notify_progress(const String& title_id) override;

  void notify_error(const String& message_id, const String& title_id = EMPTY_STRING) override;

  void show_server_selection(const std::vector<CharacterServer> servers) override;

  EngineResourceLoader& resource_loader() { return resource_loader_; }

 private:
  const GameConfig config_;
  EngineResourceLoader& resource_loader_;
  std::unique_ptr<AudioDevice> audio_device_;
};
}  // namespace crayon

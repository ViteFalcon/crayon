#include "engine/console.h"
#include "engine/engine.h"
#include "engine/resource_loader.h"
#include "game/core/string.h"
#include "game/game.h"
#include "game/game_config.h"
#include "game/log.h"

namespace crayon {
/// <summary>
/// This class is used to make use of RAII to cleanly exit the game
/// </summary>
class GameRunner {
  Console console_;
  Engine engine_;
  Game game_;

 public:
  GameRunner(ResourceLoader& resource_loader, const GameConfig& config)
      : engine_(config, resource_loader), game_(engine_, config) {}

  void run() {
    GameUpdater updator = [this](double delta_time_secs) { game_.update(delta_time_secs); };
    engine_.run(updator);
  }

#ifdef _WIN32
 private:
  UINT _original_code_page;
#endif
};
}  // namespace crayon

int main(int arg_count, char** args) {
  crayon::ResourceLoader resource_loader(arg_count > 0 ? args[0] : nullptr);
  crayon::GameConfig config(resource_loader, arg_count, args);
  crayon::GameRunner game_runner(resource_loader, config);
  try {
    game_runner.run();
  } catch (const std::exception& ex) {
    crayon::log::critical("Unexpected error. Reason: {}", ex.what());
  }
  return 0;
}

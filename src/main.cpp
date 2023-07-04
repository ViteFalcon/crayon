#include "engine/console.h"
#include "engine/engine.h"
#include "game/core/string.h"
#include "game/game.h"
#include "game/game_config.h"
#include "game/log.h"

namespace crayon {
/// <summary>
/// This class is used to make use of RAII to cleanly exit the game
/// </summary>
class GameRunner {
  Console _console;
  Engine _engine;
  Game _game;

 public:
  GameRunner(const GameConfig& config) : _engine(config), _game(_engine, config) {}

  void run() {
    GameUpdater updator = [this](double delta_time_secs) { _game.update(delta_time_secs); };
    _engine.run(updator);
  }

#ifdef _WIN32
 private:
  UINT _original_code_page;
#endif
};
}  // namespace crayon

int main(int arg_count, char** args) {
  crayon::GameConfig config(arg_count, args);
  crayon::GameRunner game_runner(config);
  try {
    game_runner.run();
  } catch (const std::exception& ex) {
    crayon::log::critical("Unexpected error. Reason: {}", ex.what());
  }
  return 0;
}

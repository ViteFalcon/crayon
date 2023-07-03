#include "engine/console.h"
#include "engine/engine.h"
#include "game/core/string.h"
#include "game/game.h"
#include "game/game_config.h"
#include "game/log.h"
#include "serdepp/adaptor/yaml-cpp.hpp"

namespace crayon {
/// <summary>
/// This class is used to make use of RAII to cleanly exit the game
/// </summary>
class GameRunner {
  Console _console;
  Engine _engine;
  Game _game;

 public:
  GameRunner(const GameConfig& config, String title) : _engine(config, title), _game(_engine, config) {}

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

int main() {
  serde::yaml cfg_node = YAML::LoadFile("data/crayon.yaml");
  crayon::GameConfig config = serde::deserialize<crayon::GameConfig>(cfg_node);
  crayon::GameRunner game_runner(config, u"Crayon - rAthena Client (很有用)");
  try {
    game_runner.run();
  } catch (const std::exception& ex) {
    crayon::log::critical("Unexpected error. Reason: {}", ex.what());
  }
  return 0;
}

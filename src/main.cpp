#include "engine/console.h"
#include "engine/engine.h"
#include "game/core/string.h"
#include "game/game.h"
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
  GameRunner(String title) : _engine(title), _game(_engine) {}

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
  crayon::GameRunner game_runner(u"Crayon - rAthena Client (很有用)");
  try {
    game_runner.run();
  } catch (const std::exception& ex) {
    crayon::log::critical("Unexpected error. Reason: {}", ex.what());
  }
  return 0;
}

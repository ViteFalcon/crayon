#pragma once
#include <optional>

#include "game_enums.h"
#include "network/client_config.h"

namespace crayon {
/// <summary>
/// A struct to store the current context
/// </summary>
class GameContext {
 public:
  /// <summary>
  /// Returns the current state of the game
  /// </summary>
  /// <returns></returns>
  virtual GameState get_state() const = 0;

  /// <summary>
  /// Returns the login server configuration, if present
  /// </summary>
  /// <returns>Login server configuration</returns>
  virtual std::optional<ClientConfig> login_server_config() const = 0;

  /// <summary>
  /// Returns the character server configuration, if present
  /// </summary>
  /// <returns>Character server configuration</returns>
  virtual std::optional<ClientConfig> character_server_config() const = 0;

  /// <summary>
  /// Returns the map server configuration, if present
  /// </summary>
  /// <returns>Map server configuration</returns>
  virtual std::optional<ClientConfig> map_server_config() const = 0;
};
}  // namespace crayon
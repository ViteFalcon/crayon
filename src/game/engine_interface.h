#pragma once
#include "game_structs.h"

namespace crayon {
class EngineInterface {
 public:
  /// <summary>
  /// Gets called to show login window
  /// </summary>
  virtual void show_auth() = 0;

  /// <summary>
  /// Notifies the user about an in-progress event
  /// </summary>
  /// <param name="title_id">ID of the title for the progress notification</param>
  virtual void notify_progress(const String& title_id) = 0;

  /// <summary>
  /// Notifies the user about an error
  /// </summary>
  /// <param name="message_id">ID of the message</param>
  /// <param name="title_id">ID of the title</param>
  virtual void notify_error(const String& message_id, const String& title_id = EMPTY_STRING) = 0;

  /// <summary>
  /// Shows the list of servers to select
  /// </summary>
  /// <param name="servers">The list of character servers</param>
  virtual void show_server_selection(const std::vector<CharacterServer> servers) = 0;
};
}  // namespace crayon

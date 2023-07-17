#pragma once
#include <filesystem>

#include "game_structs.h"

namespace crayon {
class EngineResourceLoader {
 public:
  /// <summary>
  ///
  /// </summary>
  /// <param name="bytes"></param>
  /// <param name="path"></param>
  inline void load_all_as_bytes(std::vector<char>& bytes, const String& path) {
    load_all_as_bytes(bytes, to_str(path));
  }

  virtual void load_all_as_bytes(std::vector<char>& bytes, const std::string& path) = 0;

  /// <summary>
  /// Loads the entire resource as a string
  /// </summary>
  /// <param name="path">The path to the file</param>
  /// <returns></returns>
  std::string load_all_as_string(const String& path) {
    std::vector<char> bytes;
    load_all_as_bytes(bytes, path);
    return std::string(&bytes[0], bytes.size());
  }

 protected:
  friend class GameConfig;

  virtual void set_root_dir(const std::filesystem::path& directory) { root_dir_ = directory; }

 private:
  std::filesystem::path root_dir_;
};

class GameConfig;

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

  /// <summary>
  /// Returns an interface that reads data from the Engine's resources
  /// </summary>
  /// <returns></returns>
  virtual EngineResourceLoader& resource_loader() = 0;
};
}  // namespace crayon

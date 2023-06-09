#pragma once
#include <filesystem>
#include <vector>

#include "network/client_config.h"

namespace crayon {

class GameConfig {
 public:
  GameConfig(int arg_count, char** args);

  inline auto root_directory() const { return _root_directory; }

  inline std::filesystem::path resolve_data_file(std::string file) const { return _data_directory / file; }

  inline std::filesystem::path resolve_bgm_path(std::string bgm_file) const { return _bgm_directory / bgm_file; }

  inline std::filesystem::path title_bgm_path() const { return resolve_bgm_path(_title_bgm); }

  inline std::string window_title() const { return _window_title; }

 private:
  std::filesystem::path _root_directory;
  std::filesystem::path _data_directory;
  std::filesystem::path _bgm_directory;
  std::string _title_bgm;
  std::string _window_title;
  std::vector<ClientConfig> _login_servers;
};

}  // namespace crayon
#pragma once
#include <filesystem>
#include <vector>

#include "network/client_config.h"

namespace crayon {

class EngineResourceLoader;

class GameConfig {
 public:
  GameConfig(EngineResourceLoader& resource_loader, int arg_count, char** args);

  inline auto root_directory() const { return root_directory_; }

  inline std::filesystem::path resolve_data_file(std::string file) const { return data_directory_ / file; }

  inline std::filesystem::path resolve_bgm_path(std::string bgm_file) const { return bgm_directory_ / bgm_file; }

  inline std::filesystem::path title_bgm_path() const { return resolve_bgm_path(title_bgm_); }

  inline std::string window_title() const { return window_title_; }

 private:
  EngineResourceLoader& resource_loader_;
  std::filesystem::path root_directory_;
  std::filesystem::path data_directory_;
  std::filesystem::path bgm_directory_;
  std::string title_bgm_;
  std::string window_title_;
  std::vector<ClientConfig> login_servers_;
};

}  // namespace crayon
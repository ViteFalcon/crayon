#pragma once
#include <filesystem>
#include <vector>

#include "core/asset_path.h"
#include "network/client_config.h"

namespace crayon {

class GameOptions;
class EngineResourceLoader;

class GameConfig {
 public:
  GameConfig(const GameOptions& options, EngineResourceLoader& resource_loader);

  inline auto root_directory() const { return root_directory_; }

  AssetPath resolve_data_file(std::string file) const { return data_directory_ / file; }

  inline AssetPath resolve_bgm_path(std::string bgm_file) const { return bgm_directory_ / bgm_file; }

  inline AssetPath title_bgm_path() const { return resolve_bgm_path(title_bgm_); }

  inline std::string window_title() const { return window_title_; }

 private:
  const GameOptions& options_;
  EngineResourceLoader& resource_loader_;
  std::filesystem::path root_directory_;
  AssetPath data_directory_;
  AssetPath bgm_directory_;
  std::string title_bgm_;
  std::string window_title_;
  std::vector<ClientConfig> login_servers_;
};

}  // namespace crayon
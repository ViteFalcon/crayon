#pragma once
#include <filesystem>
#include <string>

#include "core/asset_path.h"

namespace crayon {
class GameOptions {
 public:
  GameOptions(int arg_count, char** args);

  const char* arg0() const;

  inline std::string organization() const { return organization_; }

  inline std::string app_name() const { return app_name_; }

  inline std::filesystem::path root_directory() const { return root_directory_; };

  inline std::string data_directory_name() const { return data_directory_; }

  inline std::string config_file_name() const { return config_file_; }

 private:
  std::string organization_;
  std::string app_name_;
  std::filesystem::path root_directory_;
  AssetPath data_directory_;
  AssetPath config_file_;
};
}  // namespace crayon
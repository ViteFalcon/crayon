#pragma once
#include <filesystem>
#include <string>

namespace crayon {
class GameArgs {
 public:
  GameArgs(int arg_count, char** args);

  const char* arg0() const;

 private:
  std::filesystem::path root_directory_;
  std::string data_directory_;
  std::string config_file_;
};
}  // namespace crayon
#include "asset_path.h"

#include <algorithm>
#include <cctype>
#include <filesystem>
#include <regex>

namespace crayon {

inline std::string normalize_asset_path(const std::string& path) {
  static const std::regex multiple_slashes("//+");
  std::filesystem::path normalized_path{path};
  auto value = normalized_path.string();
  std::replace(value.begin(), value.end(), '\\', '/');
  value = std::regex_replace(value, multiple_slashes, "/");
  if (value.ends_with("/")) {
    value = value.substr(0, value.length() - 1);
  }
  trim(value);
  return value;
}

AssetPath::AssetPath(const std::string& path) : value_(normalize_asset_path(path)) {}

AssetPath::AssetPath(const String& path) : AssetPath(to_str(path)) {}

}  // namespace crayon

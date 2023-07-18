#pragma once
#include "string.h"

namespace crayon {

class AssetPath {
 public:
  AssetPath() = default;
  AssetPath(const std::string& path);
  AssetPath(const String& path);

  inline std::string string() const { return value_; }

  inline AssetPath operator/(const AssetPath& rhs) const { return value_ + "/" + rhs.value_; }

  inline operator std::string() const { return value_; }

 private:
  std::string value_;
};

}  // namespace crayon

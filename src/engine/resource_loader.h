#pragma
#include "../game/engine_interface.h"

namespace crayon {

class GameOptions;

class ResourceLoader : public EngineResourceLoader {
 public:
  ResourceLoader(const GameOptions& options);
  ~ResourceLoader();

  void load_all_as_bytes(std::vector<char>& bytes, const AssetPath& path) override;

  std::size_t file_size(const AssetPath& path) const;

 private:
  const GameOptions& options_;
};

}  // namespace crayon
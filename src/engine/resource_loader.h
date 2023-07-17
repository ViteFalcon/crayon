#pragma
#include "../game/engine_interface.h"

namespace crayon {
class ResourceLoader : public EngineResourceLoader {
 public:
  ResourceLoader(const char* arg0);
  ~ResourceLoader();

  void load_all_as_bytes(std::vector<char>& bytes, const std::string& path) override;

 protected:
  void set_root_dir(const std::filesystem::path& directory) override;
};
}  // namespace crayon
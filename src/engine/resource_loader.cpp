#include "resource_loader.h"

#include <physfs.h>

#include "../game/game_options.h"
#include "../game/log.h"

namespace crayon {

inline std::string get_physfs_error(std::string context) {
  std::stringstream error_message;
  error_message << context << " Reason: " << PHYSFS_getLastError();
  return error_message.str();
}

#define THROW_PHYSFS_ERROR(msg)                \
  std::string message = get_physfs_error(msg); \
  throw std::runtime_error(message)

#define VERIFY_PHYSFS_CALL(retval, error_message) \
  if (!retval) {                                  \
    THROW_PHYSFS_ERROR(error_message);            \
  }

inline void delete_physfs_file(std::string path, PHYSFS_File* file) {
  VERIFY_PHYSFS_CALL(PHYSFS_close(file), "Failed to close resource file: " + path + ".");
}

class PhysFileReader {
  std::string path_;
  PHYSFS_File* file_;

 public:
  PhysFileReader(const std::string& path) : path_(path), file_(PHYSFS_openRead(path_.c_str())) {
    if (!file_) {
      THROW_PHYSFS_ERROR("Failed to read file: " + path_ + "'.");
    }
  }

  ~PhysFileReader() noexcept {
    if (!PHYSFS_close(file_)) {
      log::error(get_physfs_error("Failed to close read-only file: " + path_ + "'."));
    }
  }

  std::size_t size() const { return PHYSFS_fileLength(file_); }

  void read_all_bytes(std::vector<char>& bytes) {
    bytes.clear();
    auto file_size = size();
    bytes.resize(file_size, 0);
    PHYSFS_readBytes(file_, &bytes[0], file_size);
  }
};

ResourceLoader::ResourceLoader(const GameOptions& options) : options_(options) {
  VERIFY_PHYSFS_CALL(PHYSFS_init(options.arg0()), "Failed to initialize PhysFS.");
  VERIFY_PHYSFS_CALL(PHYSFS_mount(options.root_directory().string().c_str(), nullptr, 0),
                     "Failed to set root directory for PhysFS.");
  VERIFY_PHYSFS_CALL(PHYSFS_setSaneConfig(options.organization().c_str(), options.app_name().c_str(), "zip", 0, 0),
                     "Failed to set sane configs.");
}

ResourceLoader::~ResourceLoader() {
  if (!PHYSFS_deinit()) {
    log::critical(get_physfs_error("Failed to deinitialize PhysFS."));
  }
}

void ResourceLoader::load_all_as_bytes(std::vector<char>& bytes, const AssetPath& path) {
  PhysFileReader reader(path.string());
  reader.read_all_bytes(bytes);
}

std::size_t ResourceLoader::file_size(const AssetPath& path) const { return PhysFileReader(path.string()).size(); }

}  // namespace crayon
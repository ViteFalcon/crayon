#include "resource_loader.h"

#include <physfs.h>

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

  ~PhysFileReader() { VERIFY_PHYSFS_CALL(PHYSFS_close(file_), "Failed to close read-only file: " + path_ + "'."); }

  void read_all_bytes(std::vector<char>& bytes) {
    auto size = PHYSFS_fileLength(file_);
    bytes.clear();
    bytes.reserve(size);
    PHYSFS_readBytes(file_, &bytes[0], size);
  }
};

ResourceLoader::ResourceLoader(const char* arg0) {
  VERIFY_PHYSFS_CALL(PHYSFS_init(arg0), "Failed to initialize PhysFS.");
}

ResourceLoader::~ResourceLoader() { VERIFY_PHYSFS_CALL(PHYSFS_deinit(), "Failed to deinitialize PhysFS."); }

void ResourceLoader::load_all_as_bytes(std::vector<char>& bytes, const std::string& path) {
  PhysFileReader reader(path);
  reader.read_all_bytes(bytes);
}

void ResourceLoader::set_root_dir(const std::filesystem::path& directory) {
  VERIFY_PHYSFS_CALL(PHYSFS_mount(directory.string().c_str(), nullptr, 0), "Failed to set root directory for PhysFS.");
}

}  // namespace crayon
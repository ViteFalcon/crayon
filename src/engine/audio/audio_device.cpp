#ifdef _WIN32
#define _WINUSER_
#define _WINGDI_
#define _IMM_
#define _APISETCONSOLEL3_
#endif  // _WIN32
#include "audio_device.h"

#include <raylib.h>

#include <boost/algorithm/string.hpp>

#include "game/engine_interface.h"

namespace crayon {

enum class MusicType { BACKGROUND, AMBIENT };

Music raylib_load_music(std::vector<char>& buffer, EngineResourceLoader& resource_loader, const std::string& path) {
  resource_loader.load_all_as_bytes(buffer, path);
  std::filesystem::path music_path(path);
  auto extension = boost::algorithm::to_lower_copy(music_path.extension().string());
  return LoadMusicStreamFromMemory(extension.c_str(), (const unsigned char*)&buffer[0], buffer.size());
}

class AudioStream {
  std::vector<char> buffer_;
  Music instance_;
  UnitFloat volume_;

 public:
  AudioStream(EngineResourceLoader& resource_loader, const std::string& path)
      : buffer_{}, instance_(raylib_load_music(buffer_, resource_loader, path)) {}

  ~AudioStream() noexcept { UnloadMusicStream(instance_); }

  bool is_ready() const { return IsMusicReady(instance_); }

  void play() { PlayMusicStream(instance_); }

  bool is_playing() const { return IsMusicStreamPlaying(instance_); }

  void stop() { StopMusicStream(instance_); }

  void set_volume(UnitFloat volume) { SetMusicVolume(instance_, volume); }

  void update() { UpdateMusicStream(instance_); }
};

AudioDevice::AudioDevice(EngineResourceLoader& resource_loader)
    : resource_loader_(resource_loader), bgm_{}, bgm_volume_(UnitFloat::MAX_VALUE) {
  ::InitAudioDevice();
}

AudioDevice::~AudioDevice() { ::CloseAudioDevice(); }

void AudioDevice::update() {
  if (bgm_) {
    bgm_->update();
  }
}

void AudioDevice::set_bgm(const AssetPath& path) {
  bgm_ = std::make_unique<AudioStream>(resource_loader_, path);
  bgm_->set_volume(bgm_volume_);
  bgm_->play();
}

void AudioDevice::set_bgm_volume(const UnitFloat& value) {
  if (bgm_) {
    bgm_->set_volume(value);
  }
  bgm_volume_ = value;
}

}  // namespace crayon
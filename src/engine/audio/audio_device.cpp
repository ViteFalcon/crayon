#include "audio_device.h"

#include <raylib.h>

#include "game/engine_interface.h"

namespace crayon {

static const std::size_t MUSIC_BUFFER_SIZE = 3693847;
static const std::size_t MUSIC_COUNT = 44;
static const std::size_t TOTAL_MUSIC_BUFFER_SIZE = MUSIC_BUFFER_SIZE * MUSIC_COUNT;
static std::vector<char> MUSIC_BUFFER(TOTAL_MUSIC_BUFFER_SIZE, 0);

enum class MusicType { BACKGROUND, AMBIENT };

Music raylib_load_music(EngineResourceLoader& resource_loader, const std::string& path) {
  std::vector<char> bytes;
  resource_loader.load_all_as_bytes(bytes, path);
  std::filesystem::path music_path(path);
  return LoadMusicStreamFromMemory(music_path.extension().string().c_str(), (const unsigned char*)&bytes[0],
                                   bytes.size());
}

class AudioStream {
  Music _instance;
  UnitFloat _volume;

 public:
  AudioStream(EngineResourceLoader& resource_loader, const std::string& path)
      : _instance(raylib_load_music(resource_loader, path)) {}

  ~AudioStream() { UnloadMusicStream(_instance); }

  bool is_ready() const { return IsMusicReady(_instance); }

  void play() { PlayMusicStream(_instance); }

  bool is_playing() const { return IsMusicStreamPlaying(_instance); }

  void stop() { StopMusicStream(_instance); }

  void set_volume(UnitFloat volume) { SetMusicVolume(_instance, volume); }

  void update() { UpdateMusicStream(_instance); }
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

void AudioDevice::set_bgm(const std::string& path) {
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
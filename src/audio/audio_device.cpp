#include "audio_device.h"

#include <raylib.h>

namespace crayon {

class AudioStream {
  Music _instance;
  UnitFloat _volume;

 public:
  AudioStream(const std::string& path) : _instance(LoadMusicStream(path.c_str())) {}
  ~AudioStream() { UnloadMusicStream(_instance); }

  bool is_ready() const { return IsMusicReady(_instance); }

  void play() { PlayMusicStream(_instance); }

  bool is_playing() const { return IsMusicStreamPlaying(_instance); }

  void stop() { StopMusicStream(_instance); }

  void set_volume(UnitFloat volume) { SetMusicVolume(_instance, volume); }

  void update() { UpdateMusicStream(_instance); }
};

AudioDevice::AudioDevice() : _bgm{},_bgm_volume(UnitFloat::MAX_VALUE) { ::InitAudioDevice(); }

AudioDevice::~AudioDevice() { ::CloseAudioDevice(); }

void AudioDevice::update() {
  if (_bgm) {
    _bgm->update();
  }
}

void AudioDevice::set_bgm(const std::string& path) {
  _bgm = std::make_unique<AudioStream>(path);
  _bgm->set_volume(_bgm_volume);
  _bgm->play();
}

void AudioDevice::set_bgm_volume(const UnitFloat& value) {
  if (_bgm) {
    _bgm->set_volume(value);
  }
  _bgm_volume = value;
}

}  // namespace crayon
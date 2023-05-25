#pragma once
#include <memory>

#include "../core/bounded_value.h"

namespace crayon {

class AudioStream;

class AudioDevice {
 public:
  AudioDevice();
  ~AudioDevice();

  AudioDevice(const AudioDevice&) = delete;
  AudioDevice& operator=(const AudioDevice&) = delete;

  void update();

  void set_bgm(const std::string& path);
  void set_bgm_volume(const UnitFloat& volume);
  UnitFloat get_bgm_volume() const { return _bgm_volume; }

 private:
  std::unique_ptr<AudioStream> _bgm;
  UnitFloat _bgm_volume;
};

}  // namespace crayon
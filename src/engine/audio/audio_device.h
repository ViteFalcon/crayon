#pragma once
#include <game/core/asset_path.h>
#include <game/core/bounded_value.h>

#include <memory>
#include <string>

namespace crayon {

class AudioStream;

class EngineResourceLoader;

class AudioDevice {
 public:
  AudioDevice(EngineResourceLoader& resource_loader);
  ~AudioDevice();

  AudioDevice(const AudioDevice&) = delete;
  AudioDevice& operator=(const AudioDevice&) = delete;

  void update();

  void set_bgm(const AssetPath& path);
  void set_bgm_volume(const UnitFloat& volume);
  UnitFloat get_bgm_volume() const { return bgm_volume_; }

 private:
  EngineResourceLoader& resource_loader_;
  std::unique_ptr<AudioStream> bgm_;
  UnitFloat bgm_volume_;
};

}  // namespace crayon
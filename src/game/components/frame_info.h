#pragma once
#include <cstdint>

namespace crayon {
struct FrameInfo {
  std::uint64_t frame_count;
  double delta_time_secs;
};
}  // namespace crayon
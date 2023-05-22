#include "string.h"

#include <cstdlib>  // MB_CUR_MAX
#include <cuchar>

namespace crayon {
constexpr inline int UTF8_CHAR_MAX_BYTES = 4;
constexpr inline auto BAD_CODE_POINT = ((size_t)-1);

StringBytes to_bytes(StringView value) {
#ifdef STANDARD_CUCHAR_IMPLEMENTED
#define TO_MULTIBYTE c8rtomb
#else
#define TO_MULTIBYTE c16rtomb
#endif

  StringBytes result{
      .data = {},
      .glyphs = {},
  };

  // Variation of https://stackoverflow.com/a/58909239/210634
  if (value.length() == 0) {
    return result;
  }

  const auto in = value.data();
  const auto code_points = value.length();
  const int max_size = UTF8_CHAR_MAX_BYTES * code_points + 1;

  std::mbstate_t state = {};
  result.glyphs.reserve(max_size);
  std::string bytes(max_size, '\0');

  char* one_char = &bytes[0];
  size_t total_bytes = 0;
  for (size_t byte_count, n = 0; n < code_points; ++n) {
    byte_count = TO_MULTIBYTE(one_char, in[n], &state);
    if (byte_count == BAD_CODE_POINT) {
      break;
    } else if (byte_count < 1) {
      continue;
    }
    result.glyphs.emplace_back(total_bytes, byte_count);
    total_bytes += byte_count;
    one_char += byte_count;
  }
  result.data.assign(&bytes[0], &bytes[0] + total_bytes);
  return result;
}
}  // namespace crayon
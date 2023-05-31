#pragma once
#include <cuchar>
#include <memory>
#include <string>
#include <vector>

namespace crayon {
#ifdef STANDARD_CUCHAR_IMPLEMENTED
using String = std::u8string;
using StringView = std::u8string_view;
#else
using String = std::u16string;
using StringView = std::u16string_view;
#endif

extern const String EMPTY_STRING;

struct CharacterIndex {
  size_t index;
  size_t bytes;
};

struct StringBytes {
  std::vector<char> data;
  std::vector<CharacterIndex> glyphs;

  inline std::string to_str() const {
    std::string result(&data[0], data.size());
    return result;
  }
};

std::string to_str(String value);

StringBytes to_bytes(StringView value);

template <size_t N>
class FixedBytesString {
 public:
  FixedBytesString() : _data{'\0'} {}
  FixedBytesString(StringBytes bytes) : _data{'\0'} {
    for (int i = 0, total_bytes = 0; i < bytes.glyphs.size() && total_bytes < N; ++i) {
      auto glyph_size = bytes.glyphs[i].bytes;
      if ((total_bytes + glyph_size) > N) {
        break;
      }
      auto glyph_index = bytes.glyphs[i].index;
      std::memcpy(&_data[total_bytes], &bytes.data[glyph_index], glyph_size);
      total_bytes += glyph_size;
    }
  }
  FixedBytesString(StringView value) : FixedBytesString(to_bytes(value)) {}

  inline const char* ptr() const { return _data; }

  bool operator==(const FixedBytesString& other) const { return strcmp(_data, other._data) == 0; }

 private:
  char _data[N];
};

}  // namespace crayon
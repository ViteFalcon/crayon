#include <iostream>

#include "core/string.h"

#ifdef _WIN32
#include <Windows.h>

#include <codecvt>
#else
using UINT = unsigned int;

UINT GetConsoleOutputCP() { return 0; }

void SetConsoleOutputCP(UINT _code_page) {}
#endif  // _WIN32

namespace crayon {
/// <summary>
/// This class is used to make use of RAII to cleanly exit the game
/// </summary>
class GameRunner {
 public:
  GameRunner() : _original_code_page(::GetConsoleOutputCP()) {
    ::SetConsoleOutputCP(CP_UTF8);
    std::setlocale(LC_ALL, "en_US.utf16");
  }

  ~GameRunner() { ::SetConsoleOutputCP(_original_code_page); }

  void run() {
    String my_str = u"很有用👍";
    std::cout << my_str.length() << std::endl;
    std::wstring_convert<std::codecvt_utf8<char16_t>, char16_t> converter;
    std::cout << converter.to_bytes(my_str) << std::endl;
    StringBytes bytes = to_bytes(my_str);
    std::cout << "Total bytes: " << bytes.data.size() << std::endl;
    for (int i = 0; i < bytes.glyphs.size(); ++i) {
      std::cout << "\t";
      std::uint64_t value = 0;
      auto glyph_index_start = bytes.glyphs[i].index;
      switch (bytes.glyphs[i].bytes) {
        case 1:
          value = bytes.data[glyph_index_start];
          break;
        default:
          std::memcpy(&value, &bytes.data[glyph_index_start], bytes.glyphs[i].bytes);
      }
      std::cout << "[" << bytes.glyphs[i].bytes << "]" << std::showbase << std::hex << value << std::dec << std::endl;
    }
    FixedBytesString<23> v(my_str);
    for (int i = 0; i < 23; ++i) {
      std::cout << std::showbase << std::hex << (int)v.ptr()[i] << " ";
    }
    std::cout << std::endl;
  }

 private:
  UINT _original_code_page;
};
}  // namespace crayon

int main() {
  crayon::GameRunner game_runner;
  try {
    game_runner.run();
  } catch (const std::exception& ex) {
    std::cout << "Unexpected error. Reason: " << ex.what() << std::endl;
  }
  return 0;
}

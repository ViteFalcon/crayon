#include "console.h"

#ifdef _WIN32
#include <Windows.h>
#endif  // _WIN32

namespace crayon {
Console::Console() {
#ifdef _WIN32
  _original_code_page = GetConsoleOutputCP();
  SetConsoleOutputCP(CP_UTF8);
#endif  // _WIN32
}

Console::~Console() {
#ifdef _WIN32
  SetConsoleOutputCP(_original_code_page);
#endif  // _WIN32
}
}  // namespace crayon

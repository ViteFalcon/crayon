#pragma once

namespace crayon {
class Console {
 public:
  Console();
  ~Console();

 private:
  unsigned int _original_code_page;
};
}  // namespace crayon
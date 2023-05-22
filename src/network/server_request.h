#pragma once
#include <cstdint>

#include "../enum_utils.h"

namespace crayon {

#define SERVER_REQUESTS    \
  X(AccountAuth, 0x0064)   \
  X(AuthKeepAlive, 0x0200) \
  X(GameAuth, 0x0065)      \
  X(CharSelect, 0x0066)    \
  X(CharCreate, 0xA39)     \
  X(CharKeepAlive, 0x0187) \
  X(RequestTime, 0x07E)

#define X(Name, Value) Name = Value,
DECLARE_ENUM(ServerRequest, std::uint16_t, SERVER_REQUESTS)
#undef X

}  // namespace crayon
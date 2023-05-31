#pragma once
#include <cstdint>
#include <vector>

#include "core/string.h"
#include "game_enums.h"
#include "network/client_config.h"

namespace crayon {

struct LoginFailed {
  LoginFailReason reason;
  std::string details;
};

struct GameAccount {
  std::string username;
  std::uint32_t account_id;
  std::uint32_t session_id;
  std::uint32_t login_id;
  CharacterGender gender;
  std::vector<std::uint8_t> web_auth_token;
};

struct CharacterServer {
  ClientConfig config;
  std::string name;
  std::uint32_t active_users;
  GameServerState state;
  GameServerBadge badge;
};

struct LoginSucceeded {
  std::uint32_t account_id;
  std::uint32_t session_id;
  std::uint32_t login_id;
  CharacterGender gender;
  std::vector<std::uint8_t> web_auth_token;
  std::vector<CharacterServer> servers;
};

}  // namespace crayon

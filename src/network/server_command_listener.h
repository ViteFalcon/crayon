#pragma once
#include <cstdint>
#include <string>
#include <vector>

#include "../game_enums.h"
#include "client_config.h"

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

class ServerCommandListener {
 public:
  virtual void on_login_success(const LoginSucceeded& response) = 0;

  virtual void on_login_failed(const LoginFailed& response) = 0;

  virtual void on_banned(BanReason reason) = 0;

  virtual void notify_error(std::string message) = 0;
};
}  // namespace crayon
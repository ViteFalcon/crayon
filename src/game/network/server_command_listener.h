#pragma once
#include "../game_structs.h"

namespace crayon {

class ServerCommandListener {
 public:
  virtual void on_login_success(const LoginSucceeded& response) = 0;

  virtual void on_login_failed(const LoginFailed& response) = 0;

  virtual void on_banned(BanReason reason) = 0;

  virtual void notify_error(std::string message) = 0;
};

}  // namespace crayon
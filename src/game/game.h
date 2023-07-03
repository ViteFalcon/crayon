#pragma once

#include <asio.hpp>
#include <atomic>
#include <chrono>
#include <thread>

#include "core/string.h"
#include "engine_interface.h"
#include "game_config.h"
#include "game_context.h"
#include "network/connection.h"
#include "network/server_command_listener.h"
#include "network/server_command_processor.h"

namespace crayon {

class MutableGameContext;

class Game : public ServerCommandListener {
 public:
  Game(EngineInterface& engine, const GameConfig& config);
  ~Game();

  const GameContext& get_context() const;

  void update(double delta);

  void async_request_login(StringView username, StringView password);

  void on_login_success(const LoginSucceeded& response) override;

  void on_login_failed(const LoginFailed& response) override;

  void on_banned(BanReason reason) override;

  void notify_error(std::string message) override;

 private:  // methods
  void _background_tasks(std::stop_token stop_token);

 private:
  EngineInterface& _engine;
  const GameConfig _config;
  std::unique_ptr<MutableGameContext> _context;
  asio::io_context _io_context;
  // This is to stop io_context from running out of work: https://tinyurl.com/asio-work-guard
  asio::executor_work_guard<asio::io_context::executor_type> _work_guard = asio::make_work_guard(_io_context);
  std::unique_ptr<std::jthread> _background_thread;
  ServerCommandProcessor _server_command_processor;
  Connection _login_connection;
  Connection _char_select_connection;
  Connection _map_connection;
  volatile bool _update_connections;
};

}  // namespace crayon

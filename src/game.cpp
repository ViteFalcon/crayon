#include "game.h"

#include <cmath>
#include <serdepp/serde.hpp>
#include <sstream>

#include "network/network_packet_builder.h"

namespace crayon {

class MutableGameContext : public GameContext {
 public:
  GameState get_state() const override { return _state; }

  std::optional<ClientConfig> login_server_config() const override { return _login_server_config; };

  std::optional<ClientConfig> character_server_config() const override { return _char_server_config; }

  std::optional<ClientConfig> map_server_config() const override { return _map_server_config; }

 public:  // These are intentially public because this will only be changed from within this game
  GameState _state = GameState::Start;
  std::optional<ClientConfig> _login_server_config;
  std::optional<ClientConfig> _char_server_config;
  std::optional<ClientConfig> _map_server_config;
};

constexpr MutableGameContext& as_ref(std::unique_ptr<GameContext>& ptr) {
  return *dynamic_cast<MutableGameContext*>(ptr.get());
}

Game::Game()
    : _server_command_processor(*this),
      _login_connection(
          _io_context, [this]() { return _context->login_server_config(); }, _server_command_processor,
          ServerRequest::AuthKeepAlive),
      _char_select_connection(
          _io_context, [this]() { return _context->character_server_config(); }, _server_command_processor,
          ServerRequest::CharKeepAlive),
      _map_connection(
          _io_context, [this]() { return _context->map_server_config(); }, _server_command_processor,
          ServerRequest::RequestTime) {
  _context = std::make_unique<MutableGameContext>();
  _background_thread = std::make_unique<std::jthread>([&]() -> void { _io_context.run(); });
  as_ref(_context)._login_server_config = ClientConfig::for_host("localhost", 6900);
}

Game::~Game() {
  _work_guard.reset();
  _io_context.stop();
  _background_thread.reset();
}

const GameContext& Game::get_context() const { return *_context; }

void Game::update(float delta) {
  _map_connection.read_one();
  _char_select_connection.read_one();
  _login_connection.read_one();
}

void Game::async_request_login(StringView username, StringView password) {
  FixedBytesString<24> login_username(username);
  FixedBytesString<24> login_password(password);
  NetworkPacketBuilder(ServerRequest::AccountAuth, _login_connection)
      .write_u32(100)
      .write_strl(login_username)
      .write_strl(login_password)
      .write_u8(1)
      .send();
  std::vector<char> username_buffer(24, 0);
  std::memcpy(&username_buffer[0], login_username.ptr(), std::min((int)username.length(), 24));
  _login_connection.set_keep_alive_writer([username_buffer](NetworkPacketBuilder& packet_builder) {
    packet_builder.write(&username_buffer[0], username_buffer.size());
  });
}

void Game::on_login_success(const LoginSucceeded& response) {
  // TODO: Trigger a signal
}

void Game::on_login_failed(const LoginFailed& response) { _login_connection.disconnect(); }

void Game::on_banned(BanReason reason) {}

void Game::notify_error(std::string message) { std::cerr << message << std::endl; }

void Game::_background_tasks(std::stop_token stop_token) {
  while (!stop_token.stop_requested()) {
    // This task will take care of sending actions from user to server
    // NOTE: This should not sync commands from the server to avoid concurrency issues
    std::this_thread::yield();
  }
}
}  // namespace crayon
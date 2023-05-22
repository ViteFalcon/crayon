#include "connection.h"

#include <iostream>

#include "server_command_listener.h"

namespace crayon {

Connection::Connection(asio::io_context& context, const ClientConfigSupplier& config_supplier,
                       ServerCommandProcessor& command_processor, ServerRequest keep_alive_request)
    : _stream(ClientConfig::Protocol::socket(context)),
      _config_supplier(config_supplier),
      _command_processor(command_processor),
      _keep_alive_processor(*this, keep_alive_request) {}

void Connection::async_write(const std::vector<char>& data) {
  if (data.empty()) {
    return;
  }

  if (!_stream) {
    auto config = _config_supplier();
    if (!config) {
      _command_processor.get_listener().notify_error("Missing client config for the connection");
      return;
    }
    _stream.connect(config.value().endpoint);
    if (!_stream) {
      _command_processor.get_listener().notify_error("Failed to connect to server!");
      return;
    }
    _keep_alive_processor.set_interval(config.value().keep_alive_interval);
  }

  // This is a bit hacky! Can there be a better way to do this?
  // Also, this needs to be verified.
  using ResponseHandler = std::function<void(asio::error_code, std::size_t)>;
  static ResponseHandler write_response_handler = [this](asio::error_code ec, std::size_t bytes_written) {
    if (ec) {
      _command_processor.get_listener().notify_error("Failed to write data. Reason: " + ec.message());
    }
  };

  asio::async_write((ClientConfig::Protocol::socket&)_stream.socket(), asio::buffer(data), write_response_handler);
}

void Connection::read_one() {
  if (!_stream) {
    return;
  }
  _keep_alive_processor.update();
  _command_processor.process(_stream, std::chrono::seconds(0));
}

void Connection::set_keep_alive_writer(KeepAlivePayloadWriter keep_alive_supplier) {
  _keep_alive_processor.set_payload_supplier(keep_alive_supplier);
}

void Connection::disconnect() { _stream.close(); }
}  // namespace crayon
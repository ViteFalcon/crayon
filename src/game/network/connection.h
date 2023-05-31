#pragma once

#include <asio.hpp>
#include <chrono>
#include <functional>
#include <optional>

#include "client_config.h"
#include "keep_alive_processor.h"
#include "server_command_processor.h"

namespace crayon {
class Connection {
 public:
  using ClientConfigSupplier = std::function<std::optional<ClientConfig>()>;
  using KeepAlivePayloadWriter = KeepAliveProcessor::PayloadWriter;

  Connection(asio::io_context& context, const ClientConfigSupplier& config_supplier,
             ServerCommandProcessor& command_processor, ServerRequest keep_alive_request);

  void async_write(const std::vector<char>& data);

  void read_one();

  void set_keep_alive_writer(KeepAlivePayloadWriter keep_alive_supplier);

  void disconnect();

 private:
  ClientConfig::Protocol::iostream _stream;
  ClientConfigSupplier _config_supplier;
  ServerCommandProcessor& _command_processor;
  KeepAliveProcessor _keep_alive_processor;
};
}  // namespace crayon
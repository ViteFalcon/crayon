#pragma once
#include <cstdint>
#include <optional>

#include "client_config.h"

namespace crayon {
class ServerCommandListener;

/**
 * Responsible for processing server commands
 *
 * This class is responsible to abstract away the interactions with the network bytes parsing implementation.
 *
 * As of this writing, the library used for parsing network bytes is <a href="https://kaitai.io/">KaitaiStruct</a>
 */
class ServerCommandProcessor {
 public:
  using optional_timeout = std::optional<std::chrono::milliseconds>;

  ServerCommandProcessor(ServerCommandListener& listener);

  /**
   * Processes server commands and notifies the listener
   */
  bool process(ClientConfig::Protocol::iostream& stream, optional_timeout timeout = std::nullopt);

  inline ServerCommandListener& get_listener() { return _listener; }

 private:
  ServerCommandListener& _listener;
};

}  // namespace crayon
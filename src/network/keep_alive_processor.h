#pragma once
#include <chrono>
#include <functional>
#include <vector>

#include "server_request.h"

namespace crayon {
class Connection;
class NetworkPacketBuilder;

class KeepAliveProcessor {
 public:
  using PayloadWriter = std::function<void(NetworkPacketBuilder &)>;

  KeepAliveProcessor(Connection &connection, ServerRequest keep_alive_request);

  void update();

  void reset();

  void set_interval(std::chrono::seconds interval);

  void set_payload_supplier(const PayloadWriter &payload_supplier);

 private:
  using Clock = std::chrono::system_clock;
  using TimePoint = Clock::time_point;

  static const PayloadWriter DEFAULT_PAYLOAD_WRITER;

  Connection &_connection;
  const ServerRequest _request;
  PayloadWriter _payload_supplier;
  Clock::duration _keep_alive_interval;
  TimePoint _last_keep_alive_time;
};
}  // namespace crayon
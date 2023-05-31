#include "keep_alive_processor.h"

#include "connection.h"
#include "keep_alive_processor.h"
#include "network_packet_builder.h"

namespace crayon {
const KeepAliveProcessor::PayloadWriter KeepAliveProcessor::DEFAULT_PAYLOAD_WRITER =
    [](NetworkPacketBuilder &packet_builder) {
      // Does nothing
    };

KeepAliveProcessor::KeepAliveProcessor(Connection &connection, ServerRequest keep_alive_request)
    : _connection(connection),
      _request(keep_alive_request),
      _payload_supplier(DEFAULT_PAYLOAD_WRITER),
      _keep_alive_interval(10),
      _last_keep_alive_time(Clock::now()) {}

void KeepAliveProcessor::update() {
  const auto current_time = Clock::now();
  if ((current_time - _last_keep_alive_time) < _keep_alive_interval) {
    return;
  }
  NetworkPacketBuilder packet_builder(_request, _connection);
  _payload_supplier(packet_builder);
  packet_builder.send();
  _last_keep_alive_time = current_time;
}

void KeepAliveProcessor::reset() { _last_keep_alive_time = Clock::now(); }

void KeepAliveProcessor::set_interval(std::chrono::seconds interval) {
  _keep_alive_interval = std::chrono::duration_cast<Clock::duration>(interval);
  reset();
}

void KeepAliveProcessor::set_payload_supplier(const PayloadWriter &payload_supplier) {
  _payload_supplier = payload_supplier;
}
}  // namespace crayon

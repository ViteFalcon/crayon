#include "server_command_processor.h"

#include <kaitai/kaitaistruct.h>
#include <server_packets.h>

#include <functional>
#include <unordered_map>

#include "login_events.h"
#include "server_command_listener.h"

namespace crayon {
/**
 * Processes the parsed payload.
 *
 * \return @code true if the payload was processed, @code false otherwise.
 */
bool process_payload(server_packets_t& packet, ServerCommandListener& listener);

ServerCommandProcessor::ServerCommandProcessor(ServerCommandListener& listener) : _listener(listener) {}

static const std::function<bool()> ALWAYS_TRUE = []() -> bool { return true; };

bool ServerCommandProcessor::process(ClientConfig::Protocol::iostream& iostream, optional_timeout timeout) {
  size_t bytes_available = iostream.socket().available();
  if (bytes_available < 2) {
    return false;
  }

  using clock = std::chrono::high_resolution_clock;
  std::optional<clock::duration> timeout_ns = timeout.transform([](std::chrono::milliseconds value) -> clock::duration {
    return std::chrono::duration_cast<clock::duration>(value);
  });
  const auto& has_more_time = timeout_ns
                                  .transform([](clock::duration timeout) {
                                    const auto start_time = clock::now();
                                    std::function<bool()> time_checker = [start_time, timeout]() -> bool {
                                      return (clock::now() - start_time) < timeout;
                                    };
                                    return time_checker;
                                  })
                                  .value_or(ALWAYS_TRUE);
  do {
    kaitai::kstream stream(&iostream);
    std::function<void()> discard_remaining_bytes = [&iostream, bytes_available]() {
      std::vector<char> buffer(bytes_available - 2);
      iostream.read(&buffer[0], buffer.size());
    };
    server_packets_t packet(&stream);
    bool is_unknown_payload = dynamic_cast<server_packets_t::unknown_payload_t*>(packet.payload()) != nullptr;
    if (is_unknown_payload || process_payload(packet, _listener)) {
      std::vector<char> buffer(iostream.socket().available());
      iostream.read(&buffer[0], buffer.size());
      // TODO: Log the unknown payload to debug the problem
      return false;
    }
  } while (has_more_time() && (bytes_available = iostream.socket().available()) >= 2);
  return true;
}

class PayloadProcessor {
 public:
  virtual bool process(kaitai::kstruct* payload, ServerCommandListener& listener) = 0;
};

template <class PacketType>
class PayloadProcessorT : public PayloadProcessor {
 public:
  using PacketProcessor = std::function<void(PacketType& packet, ServerCommandListener& listener)>;
  PayloadProcessorT(PacketProcessor& processor) : _processor(processor) {}

  bool process(kaitai::kstruct* payload, ServerCommandListener& listener) override {
    auto packet = dynamic_cast<PacketType*>(payload);
    if (packet == nullptr) {
      return false;
    }
    _processor(*packet, listener);
    return true;
  }

 private:
  PacketProcessor _processor;
};

using PayloadProcessorPtr = std::shared_ptr<PayloadProcessor>;
template <class PacketType>
PayloadProcessorPtr make_processor(typename PayloadProcessorT<PacketType>::PacketProcessor processor) {
  return make_shared<PayloadProcessorT<PacketType>>(processor);
}

using packet_id = server_packets_t::packet_id_t;
using payload = server_packets_t;

/**
 * Mapping of server commands to their processors
 *
 * The reason a map is being used instead of a switch is because it simplifies the code of @code process_payload
 * function. This is definitely not that readable, but hopefully doesn't slow development either.
 */
static const std::unordered_map<server_packets_t::packet_id_t, PayloadProcessorPtr> PACKET_PROCESSORS = {
    {packet_id::PACKET_ID_LOGIN_DETAILS, make_processor<payload::login_details_payload_t>(on_login_success)},
    {packet_id::PACKET_ID_LOGIN_FAILED, make_processor<payload::login_failed_payload_t>(on_login_fail)},
    {packet_id::PACKET_ID_BAN_NOTIFICATION, make_processor<payload::ban_notification_payload_t>(on_ban_notification)},
};

bool process_payload(server_packets_t& packet, ServerCommandListener& listener) {
  auto itr = PACKET_PROCESSORS.find(packet.packet_id());
  if (itr == PACKET_PROCESSORS.end()) {
    // TODO: Log that packet processor is missing
    return false;
  }
  itr->second->process(packet.payload(), listener);
  return true;
}
}  // namespace crayon
#pragma once
#include <asio/ip/tcp.hpp>
#include <chrono>

namespace crayon {
struct ClientConfig {
  using Protocol = asio::ip::tcp;
  using Endpoint = Protocol::endpoint;

  Endpoint endpoint;
  std::chrono::seconds read_timeout{60};
  std::chrono::seconds keep_alive_interval{10};

  static ClientConfig for_host(const std::string host, const std::uint16_t port) {
    using namespace asio::ip;
    return {
        .endpoint{address::from_string(host), port},
    };
  }

  static ClientConfig for_ipv4_endpoint(const std::uint32_t ip, const std::uint16_t port) {
    using namespace asio::ip;
    return {
        .endpoint{address_v4(ip), port},
    };
  }
};
}  // namespace crayon

#pragma once
#include <serdepp/serde.hpp>

namespace crayon {
struct LoginServerInfo {
  DERIVE_SERDE(LoginServerInfo, (&Self::name, "name"), (&Self::host, "host"), (&Self::port, "port"))

  std::string name;
  std::string host;
  std::uint16_t port;
};

struct GameConfig {
  DERIVE_SERDE(GameConfig, (&Self::servers, "servers"), (&Self::bgm_directory, "bgm_directory"),
               (&Self::title_bgm, "title_bgm"))

  std::vector<LoginServerInfo> servers;
  std::string bgm_directory;
  std::string title_bgm;
};
}  // namespace crayon
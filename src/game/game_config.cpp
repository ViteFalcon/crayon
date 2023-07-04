#include "game_config.h"
#include <serdepp/serde.hpp>
#include <boost/program_options.hpp>
#include <serdepp/adaptor/yaml-cpp.hpp>

namespace crayon {
struct ServerEndpoint {
  DERIVE_SERDE(ServerEndpoint, (&Self::name, "name") (&Self::host, "host") (&Self::port, "port"))

  std::string name;
  std::string host;
  std::uint16_t port;
};

struct GameConfigData {
  DERIVE_SERDE(GameConfigData, (&Self::servers, "servers") (&Self::bgm_directory, "bgm_directory")
               (&Self::title_bgm, "title_bgm"))

  std::vector<ServerEndpoint> servers;
  std::string bgm_directory;
  std::string title_bgm;
};

/// <summary>
/// Parses arguments passed to the game
/// </summary>
class GameArgs {
 public:
  std::string root_directory;

  GameArgs(int arg_count, char** args) {
    namespace po = boost::program_options;

    po::options_description program_options("Crayon options");

    program_options.add_options()(
        "root-dir,d", po::value<std::string>(&root_directory)->default_value(std::filesystem::current_path().string()),
        "Root directory");

    po::variables_map variables;
    po::store(po::parse_command_line(arg_count, args, program_options), variables);
    po::notify(variables);
  }
};

GameConfig::GameConfig(int arg_count, char** args) {
  GameArgs game_args(arg_count, args);
  _root_directory = std::filesystem::path(game_args.root_directory);
  _data_directory = _root_directory / "data";
  auto config_file_path = resolve_data_file("crayon.yaml");
  serde::yaml cfg_node = YAML::LoadFile(config_file_path.string());
  GameConfigData config = serde::deserialize<GameConfigData>(cfg_node);
  _bgm_directory = _root_directory / config.bgm_directory;
  _title_bgm = config.title_bgm;
  _login_servers.reserve(config.servers.size());
  for (const auto& itr : config.servers) {
    auto server_config = ClientConfig::for_host(itr.host, itr.port);
    server_config.name = itr.name;
    _login_servers.push_back(server_config);
  }
}
}  // namespace crayon

#include "game_config.h"

#include <fmt/format.h>

#include <boost/program_options.hpp>
#include <serdepp/adaptor/yaml-cpp.hpp>
#include <serdepp/serde.hpp>

#include "engine_interface.h"
#include "game_options.h"

namespace crayon {
struct ServerEndpoint {
  DERIVE_SERDE(ServerEndpoint, (&Self::name, "name")(&Self::host, "host")(&Self::port, "port"))

  std::string name;
  std::string host;
  std::uint16_t port;
};

struct GameConfigData {
  DERIVE_SERDE(GameConfigData, (&Self::servers, "servers")(&Self::bgm_directory, "bgm_directory")(
                                   &Self::title_bgm, "title_bgm")(&Self::window_title, "window_title"))

  std::vector<ServerEndpoint> servers;
  std::string bgm_directory;
  std::string title_bgm;
  std::string window_title;
};

/// <summary>
/// Parses arguments passed to the game
/// </summary>
class GameArgs {
 public:
  std::string root_directory;
  std::string data_directory;
  std::string config_file;

  GameArgs(int arg_count, char** args) {
    namespace po = boost::program_options;

    po::options_description program_options("Crayon options");

    program_options.add_options()(
        "root-dir,r", po::value<std::string>(&root_directory)->default_value(std::filesystem::current_path().string()),
        "Root directory")("config-file,c", po::value<std::string>(&config_file)->default_value("crayon.yaml"))(
        "data-dir,d", po::value<std::string>(&data_directory)->default_value("data"));

    po::variables_map variables;
    po::store(po::parse_command_line(arg_count, args, program_options), variables);
    po::notify(variables);
  }
};

inline std::filesystem::path validate_directory(std::filesystem::path directory, std::string name) {
  if (!std::filesystem::exists(directory)) {
    auto message = fmt::format("'{}' directory '{}' does not exist!", name, directory.string());
    throw std::invalid_argument(message);
  } else if (!std::filesystem::is_directory(directory)) {
    auto message =
        fmt::format("Value specified as '{}' directory ({}) is not a valid directory!", name, directory.string());
    throw std::invalid_argument(message);
  }
  return directory;
}

inline std::filesystem::path validate_file(std::filesystem::path path, std::string name) {
  if (!std::filesystem::exists(path)) {
    auto message = fmt::format("'{}' file does not exist in path '{}'", name, path.string());
    throw std::invalid_argument(message);
  } else if (!std::filesystem::is_regular_file(path)) {
    auto message = fmt::format("Value specified as '{}' file ({}) is not a valid file!", name, path.string());
    throw std::invalid_argument(message);
  }
  return path;
}

GameConfig::GameConfig(const GameOptions& options, EngineResourceLoader& resource_loader)
    : options_(options), resource_loader_(resource_loader) {
  root_directory_ = options.root_directory();
  data_directory_ = AssetPath(options_.data_directory_name());
  auto config_file_path = data_directory_ / options_.config_file_name();
  auto cfg_yaml = resource_loader_.load_all_as_string(config_file_path);
  serde::yaml cfg_node = YAML::Load(cfg_yaml.c_str());
  GameConfigData config = serde::deserialize<GameConfigData>(cfg_node);
  bgm_directory_ = AssetPath(config.bgm_directory);
  title_bgm_ = config.title_bgm;
  window_title_ = config.window_title;
  login_servers_.reserve(config.servers.size());
  for (const auto& itr : config.servers) {
    auto server_config = ClientConfig::for_host(itr.host, itr.port);
    server_config.name = itr.name;
    login_servers_.push_back(server_config);
  }
}

}  // namespace crayon

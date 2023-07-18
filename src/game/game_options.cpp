#include "game_options.h"

#include <fmt/format.h>

#include <boost/program_options.hpp>

namespace crayon {

const std::string ARG_0 = "";

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

GameOptions::GameOptions(int arg_count, char** args) : data_directory_("data"), config_file_("crayon.yaml") {
  namespace po = boost::program_options;

  po::options_description program_options("Crayon options");

  std::string root_directory, data_directory, config_file;
  program_options.add_options()("organization,o", po::value<std::string>(&organization_)->default_value("OlympianZ"))(
      "appname,a", po::value<std::string>(&app_name_)->default_value("crayon"))(
      "root-dir,r", po::value<std::string>(&root_directory)->default_value(std::filesystem::current_path().string()),
      "Root directory")("config-file,c", po::value<std::string>(&config_file)->default_value(config_file_.string()))(
      "data-dir,d", po::value<std::string>(&data_directory)->default_value(data_directory_.string()));

  po::variables_map variables;
  po::store(po::parse_command_line(arg_count, args, program_options), variables);
  po::notify(variables);

  root_directory_ = validate_directory(std::filesystem::path(root_directory), "Root");
  data_directory_ = data_directory;
  config_file_ = config_file;

  std::string& arg0 = const_cast<std::string&>(ARG_0);
  if (arg_count) {
    arg0 = args[0];
  } else {
    arg0 = (root_directory_ / app_name_).string();
  }
}

const char* GameOptions::arg0() const { return ARG_0.c_str(); }

}  // namespace crayon
// #include "JsonParser.hpp"

#include <fstream>
#include <nlohmann/json.hpp>

using namespace nlohmann;

json config;

void loadConfig() {
  std::ifstream ifs("etrc-2024/settings.json");
  config = json::parse(ifs);
}

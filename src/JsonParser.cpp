// #include "JsonParser.hpp"

#include <fstream>
#include <nlohmann/json.hpp>

using namespace nlohmann;

json config;

void loadConfig(const char *filename) {
  std::ifstream ifs(filename);
  config = json::parse(ifs);
}

//#include "JsonParser.hpp"

#include <nlohmann/json.hpp>
#include <fstream>

using namespace nlohmann;

json config;

void loadConfig()
{
    std::ifstream ifs("settings.json");
    config = json::parse(ifs);
}

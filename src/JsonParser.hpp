#ifndef _JSONPARSER_HPP_
#define _JSONPARSER_HPP_

#include <nlohmann/json.hpp>
#include <fstream>

using namespace nlohmann;

extern json config;

extern void loadConfig(const char *filename);

#endif // _JSONPARSER_HPP_
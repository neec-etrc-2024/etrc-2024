#ifndef JSONWRAPPER_HPP
#define JSONWRAPPER_HPP

#include <fstream>
#include <nlohmann/json.hpp>
#include <optional>

inline nlohmann::json loadConfig(const char *path) {
  std::ifstream ifs(path);
  return nlohmann::json::parse(ifs);
  ifs.close();
}

class JsonWrapper {
public:
  // コンストラクタでJSONオブジェクトを保持
  JsonWrapper(const nlohmann::json &json_obj) : json_data(json_obj) {}

  // string型の値を取得
  std::optional<std::string> getValue(const std::string &key) const {
    if (json_data.contains(key) && json_data[key].is_string()) {
      return json_data[key].get<std::string>();
    }
    return std::nullopt; // 値が存在しないか型が違う場合
  }

  // int型の値を取得
  std::optional<int> getValueInt(const std::string &key) const {
    if (json_data.contains(key) && json_data[key].is_number_integer()) {
      return json_data[key].get<int>();
    }
    return std::nullopt;
  }

  // bool型の値を取得
  std::optional<bool> getValueBool(const std::string &key) const {
    if (json_data.contains(key) && json_data[key].is_boolean()) {
      return json_data[key].get<bool>();
    }
    return std::nullopt;
  }

  // double型の値を取得
  std::optional<double> getValueDouble(const std::string &key) const {
    if (json_data.contains(key) && json_data[key].is_number()) {
      return json_data[key].get<double>();
    }
    return std::nullopt;
  }

  // array型の値を取得
  std::optional<nlohmann::json> getValueArray(const std::string &key) const {
    if (json_data.contains(key) && json_data[key].is_array()) {
      return json_data[key];
    }
    return std::nullopt;
  }

  // オブジェクト型の値を取得
  std::optional<nlohmann::json> getValueObject(const std::string &key) const {
    if (json_data.contains(key) && json_data[key].is_object()) {
      return json_data[key];
    }
    return std::nullopt;
  }

  void setStringValue(const std::string &key, const std::string &value) {
    json_data[key] = value;
  }

private:
  nlohmann::json json_data;
};

#endif // JSONWRAPPER_HPP
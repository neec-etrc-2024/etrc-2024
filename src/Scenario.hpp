#ifndef SCENARIO_HPP
#define SCENARIO_HPP

#include "JsonWrapper.hpp"
#include "Pid.hpp"
#include "informations/CameraLineMonitor.hpp"
#include <string>

extern "C" {
#include "spike/pup/motor.h"
}

using json = nlohmann::json;

enum CheckMethod { DISTANCE, BLUE };
enum CheckMode { OVER, UNDER }; // 指定値を超えるか、指定値を下回るか
enum Edge { LEFT, RIGHT };

class Scenario {
private:
  std::string name;
  bool enable;
  Pid pid_profile;
  int end;
  CheckMethod mode;
  CheckMode check_mode;
  pup_motor_t *left;
  pup_motor_t *right;
  informations::CameraLineMonitor *line_monitor;
  bool init;
  Edge edge;
  bool debug;

public:
  Scenario(JsonWrapper &json_data, pup_motor_t *left, pup_motor_t *right,
           informations::CameraLineMonitor *line_monitor)
      : name(json_data.getValue("name").value()),
        enable(json_data.getValueBool("enable").value()),
        pid_profile(left, right, json_data.getValueDouble("p").value(),
                    json_data.getValueDouble("i").value(),
                    json_data.getValueDouble("d").value(),
                    json_data.getValueInt("base_speed").value()),
        end(json_data.getValueInt("end").value()),
        mode(json_data.getValue("mode").value() == "distance" ? DISTANCE
                                                              : BLUE),
        check_mode(json_data.getValue("check_mode").value() == "over" ? OVER
                                                                      : UNDER),
        left(left), right(right), line_monitor(line_monitor), init(false),
        edge(json_data.getValue("edge").value() == "LEFT" ? LEFT : RIGHT),
        debug(json_data.getValueBool("debug")) {}

  bool run();
};

#endif // SCENARIO_HPP
#ifndef SCENARIO_RUNNER_HPP
#define SCENARIO_RUNNER_HPP

#include "JsonWrapper.hpp"
#include "Scenario.hpp"
#include <vector>

class ScenarioRunner {
private:
  std::vector<Scenario> scenarios;
  int current_scenario = 0;
  pup_motor_t *left;
  pup_motor_t *right;

public:
  ScenarioRunner(const nlohmann::json &json_data, pup_motor_t *left,
                 pup_motor_t *right,
                 informations::CameraLineMonitor *line_monitor)
      : left(left), right(right) {
   for (size_t i = 0; i < json_data.size(); ++i) {
            // JsonWrapper オブジェクトを作成し、Scenario コンストラクタに渡す
            JsonWrapper json_wrapper(json_data[i]);
            Scenario s(json_wrapper, left, right, line_monitor);
            scenarios.push_back(s);
        }
  }

  bool run() {
    bool result = scenarios[current_scenario].run();
    if (result) {
      current_scenario++;
    }
    return current_scenario >= scenarios.size();
  }
};

#endif // SCENARIO_RUNNER_HPP
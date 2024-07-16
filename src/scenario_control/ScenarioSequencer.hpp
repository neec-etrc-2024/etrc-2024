#ifndef _SCENARIO_SEQUENCER_HPP_
#define _SCENARIO_SEQUENCER_HPP_

#include "scenario_control/Scenario.hpp"
#include <vector>

namespace scenario_control {
class ScenarioSequencer {
private:
  std::vector<Scenario &> scenarios;
  std::size_t current_scenario_index;

  void change_scenario();

public:
  ScenarioSequencer(/* args */);
  void run();

  void append(Scenario &);
  ~ScenarioSequencer();
};

ScenarioSequencer::ScenarioSequencer(/* args */) {}

ScenarioSequencer::~ScenarioSequencer() {}

} // namespace scenario_control

#endif // _SCENARIO_SEQUENCER_HPP_
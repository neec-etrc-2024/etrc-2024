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
  ScenarioSequencer();
  void run();

  void append(Scenario &);
  ~ScenarioSequencer();
};

ScenarioSequencer::ScenarioSequencer()
    : scenarios(), current_scenario_index(0) {}

ScenarioSequencer::~ScenarioSequencer() {}

} // namespace scenario_control

#endif // _SCENARIO_SEQUENCER_HPP_
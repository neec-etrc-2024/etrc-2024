#include "scenario_control/ScenarioSequencer.hpp"

void scenario_control::ScenarioSequencer::run() {
  if (this->scenarios.size() == 0 ||
      this->current_scenario_index >= this->scenarios.size()) {
    return;
  }

  bool result = this->scenarios[this->current_scenario_index].run();
  if (result) {
    this->change_scenario();
  }
}

void scenario_control::ScenarioSequencer::append(
    scenario_control::Scenario &scenario) {
  this->scenarios.push_back(scenario);
}

void scenario_control::ScenarioSequencer::change_scenario() {
  this->current_scenario_index++;
}

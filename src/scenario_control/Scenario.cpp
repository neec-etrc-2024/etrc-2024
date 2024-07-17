#include "scenario_control/Scenario.hpp"

void scenario_control::Scenario::append(scenario_control::Scene &scene) {
  this->scenes.push_back(scene);
}

bool scenario_control::Scenario::run() {
  if (this->scenes.size() == 0 ||
      this->current_scene_index >= this->scenes.size()) {
    return true;
  }

  bool result = this->scenes[this->current_scene_index].run();
  if (result) {
    this->change_scene();
  }

  return false;
}

void scenario_control::Scenario::change_scene() { this->current_scene_index++; }
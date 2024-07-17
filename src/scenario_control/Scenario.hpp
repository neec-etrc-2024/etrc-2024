#ifndef _SCENARIO_HPP_
#define _SCENARIO_HPP_

#include "scenario_control/Scene.hpp"
#include <vector>

namespace scenario_control {
class Scenario {
private:
  std::vector<Scene &> scenes;
  std::size_t current_scene_index;

  void change_scene();

public:
  Scenario();
  void append(Scene &scene);
  bool run();
  ~Scenario();
};

Scenario::Scenario() : scenes(), current_scene_index(0) {}

Scenario::~Scenario() {}

} // namespace scenario_control

#endif // _SCENARIO_HPP_
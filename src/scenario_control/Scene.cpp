#include "scenario_control/Scene.hpp"

using namespace scenario_control;

bool Scene::run() {
  this->behavior.run();
  return this->condition.isCompleted();
}

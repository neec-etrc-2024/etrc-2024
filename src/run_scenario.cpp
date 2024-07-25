#include "app.hpp"
#include "devices/RunMotorController.hpp"
#include "scenario_control/Scenario.hpp"
#include "scenario_control/Scene.hpp"

using namespace scenario_control;
using namespace devices;

Scenario *create_run_scenario(RunMotorController *motor) {
  Scenario *scenario = new Scenario();
}
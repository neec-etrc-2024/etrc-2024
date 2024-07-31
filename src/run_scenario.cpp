#include "ColorSensor.hpp"
#include "Hub.hpp"
#include "app.hpp"
#include "behaviors/LineTrace.hpp"
#include "behaviors/Stop.hpp"
#include "behaviors/Straight.hpp"
#include "conditions/ButtonCondition.hpp"
#include "devices/RunMotorController.hpp"
#include "informations/ColorSensorLineMonitor.hpp"
#include "scenario_control/Scenario.hpp"
#include "scenario_control/Scene.hpp"

using namespace scenario_control;
using namespace devices;
using namespace behaviors;
using namespace conditions;
using namespace spikeapi;
using namespace informations;

Scenario *create_run_scenario(RunMotorController *motor, Hub *hub,
                              ColorSensor *color_sensor) {
  Scenario *scenario = new Scenario();
  Stop *stop = new Stop(*motor);
  ButtonCondition *leftbtn = new ButtonCondition(HUB_BUTTON_LEFT, *hub);
  Scene *stop_scene = new Scene(*stop, *leftbtn);
  scenario->append(stop_scene);

  ColorSensorLineMonitor *line_monitor =
      new ColorSensorLineMonitor(*color_sensor);
  LineTrace *line_trace =
      new LineTrace(LEFT, *motor, *line_monitor, 50, 0.5, 0.0, 0.0);
  ButtonCondition *rightbtn = new ButtonCondition(HUB_BUTTON_RIGHT, *hub);
  Scene *line_trace_scene = new Scene(*line_trace, *rightbtn);

  scenario->append(line_trace_scene);

  return scenario;
}
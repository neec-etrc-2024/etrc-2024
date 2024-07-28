#include "app.hpp"
#include "ColorSensor.hpp"
#include "Hub.hpp"
#include "Motor.hpp"
#include "devices/RunMotorController.hpp"
#include "scenario_control/Scenario.hpp"
#include <stdio.h>

extern "C" {
#include "spike/hub/imu.h"
}

using namespace spikeapi;
using namespace scenario_control;
using namespace devices;

Scenario *create_run_scenario(RunMotorController *motor, Hub *hub,
                              ColorSensor *color_sensor);

Scenario *sce = nullptr;

Motor *left_wheel = nullptr;
Motor *right_wheel = nullptr;

Hub *hub = nullptr;
ColorSensor *color_sensor = nullptr;

RunMotorController *run_motor_controller = nullptr;

void init_device();
void show_debug();

/* メインタスク(起動時にのみ関数コールされる) */
void main_task(intptr_t unused) {
  init_device();
  sce = create_run_scenario(run_motor_controller, hub, color_sensor);
  show_debug();
  sta_cyc(UPDATE_TASK_CYC);
  sta_cyc(CALC_TASK_CYC);
  sta_cyc(SET_PARAM_TASK_CYC);

  dly_tsk(600 * 1000 * 1000);

  printf("Program End\n");
  stp_cyc(UPDATE_TASK_CYC);
  stp_cyc(CALC_TASK_CYC);
  stp_cyc(SET_PARAM_TASK_CYC);

  ext_tsk();
}

void update_task(intptr_t unused) { // 情報更新
  run_motor_controller->update();
  ext_tsk();
}

void calc_task(intptr_t unused) { // 判断
  sce->run();

  ext_tsk();
}

void set_param_task(intptr_t unused) { // パラメータ設定
  run_motor_controller->apply_pwm();
  ext_tsk();
}

void init_device() {
  left_wheel = new Motor(PBIO_PORT_ID_E);
  right_wheel = new Motor(PBIO_PORT_ID_B);

  left_wheel->setup(PUP_DIRECTION_COUNTERCLOCKWISE);
  right_wheel->setup(PUP_DIRECTION_CLOCKWISE);

  run_motor_controller = new devices::RunMotorController(
      *left_wheel, *right_wheel, MOTORPOWER_SEM, MOTORCOUNT_SEM);
  run_motor_controller->init();
  hub = new spikeapi::Hub();
  color_sensor = new ColorSensor(PBIO_PORT_ID_C);
  hub->init();
}

void show_debug() {
  while (1) {
    pup_color_rgb_t rgb = color_sensor->get_rgb();
    printf("R:%d G:%d B:%d\n", rgb.r, rgb.g, rgb.b);

    if (hub->is_button_pressed(HUB_BUTTON_RIGHT)) {
      break;
    }

    tslp_tsk(1 * 1000);
  }
}

#include "app.hpp"
#include "Hub.hpp"
#include "Motor.hpp"
#include "devices/RunMotorController.hpp"
#include <stdio.h>

extern "C" {
#include "spike/hub/imu.h"
}

using namespace spikeapi;
Motor *left_wheel = nullptr;
Motor *right_wheel = nullptr;

spikeapi::Hub *hub = nullptr;

devices::RunMotorController *run_motor_controller = nullptr;

void init_device();

/* メインタスク(起動時にのみ関数コールされる) */
void main_task(intptr_t unused) {
  init_device();
  hub = new spikeapi::Hub();
  hub->init();

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
  // run_motor_controller->update();
  ext_tsk();
}

void calc_task(intptr_t unused) { // 判断
  run_motor_controller->set_pwm(30, 30);
  bool left = hub->is_button_pressed(HUB_BUTTON_LEFT);
  bool right = hub->is_button_pressed(HUB_BUTTON_RIGHT);
  bool center = hub->is_button_pressed(HUB_BUTTON_CENTER);

  printf("left: %d, right: %d, center: %d\n", left, right, center);

  ext_tsk();
}

void set_param_task(intptr_t unused) { // パラメータ設定
  run_motor_controller->apply_pwm();
  ext_tsk();
}

void init_device() {
  left_wheel = new Motor(PBIO_PORT_ID_A);
  right_wheel = new Motor(PBIO_PORT_ID_B);

  left_wheel->setup(PUP_DIRECTION_COUNTERCLOCKWISE);
  right_wheel->setup(PUP_DIRECTION_CLOCKWISE);

  run_motor_controller = new devices::RunMotorController(
      *left_wheel, *right_wheel, MOTORPOWER_SEM, MOTORCOUNT_SEM);
  run_motor_controller->init();
}

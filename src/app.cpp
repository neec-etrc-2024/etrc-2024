#include "app.hpp"
#include "FrameBuffer.hpp"
#include "JsonWrapper.hpp"
#include "ScenarioRunner.hpp"
#include "devices/CameraController.hpp"
#include "gui/WindowManager.hpp"
#include "informations/CameraLineMonitor.hpp"
#include "raspike_port.h"
#include <chrono>
#include <csignal>
#include <cstdio>
#include <lccv.hpp>
#include <mutex>
#include <opencv2/opencv.hpp>
#include <thread>
#include <vector>

extern "C" {
#include "spike/hub/button.h"
#include "spike/pup/colorsensor.h"
#include "spike/pup/motor.h"
#include "spike/pup/ultrasonicsensor.h"
}

#include "Pid.hpp"

pup_motor_t *left = nullptr;
pup_motor_t *right = nullptr;
pup_device_t *ultrasonic_sensor = nullptr;
informations::CameraLineMonitor line_monitor;

WindowManager wm;
CameraController cam;
FrameBuffer fb;

JsonWrapper *config;

ScenarioRunner *runner;

bool window_enabled = false;

void read_json(const char *path) {
  std::ifstream ifs(path);
  json data = nlohmann::json::parse(ifs);
  config = new JsonWrapper(data);
  ifs.close();
  if (config->getValue("mode") == "R") {
    printf("R mode\n");
    for (auto &elem : data["scenarios"]) {
      if (elem["edge"] == "LEFT") {
        elem["edge"] = "RIGHT";
      } else {
        elem["edge"] = "LEFT";
      }
    }
    delete config;
    config = new JsonWrapper(data);
  } else {
    printf("L mode\n");
  }

  json arr = config->getValueArray("scenarios").value();
  for (auto &elem : arr) {
    JsonWrapper json_wrapper(elem);
    std::string mode = json_wrapper.getValue("edge").value();
    printf("edge: %s\n", mode.c_str());
  }
}

void setup_scenario();
void run();
void setup_motor();
void setup_ultrasonic_sensor();
void calc();
void wait_start();
void stop();
void wait_start_btn();

void main_task(intptr_t unused) { // main_task 最初に呼ばれる

  read_json("etrc-2024/settings.json");

  setup_motor();
  setup_ultrasonic_sensor();

  setup_scenario();

  window_enabled = config->getValueBool("window").value();

  printf("init\n");

  std::vector<std::thread> threads;

  // シグナルをマスク
  sigset_t mask;
  sigemptyset(&mask);
  sigaddset(&mask, SIGUSR2);
  sigaddset(&mask, SIGALRM);
  sigaddset(&mask, SIGPOLL);
  sigaddset(&mask, SIGIO);

  pthread_sigmask(SIG_BLOCK, &mask,
                  NULL); // スレッド生成中はシグナル割り込みを無視
  if (window_enabled) {
    threads.emplace_back(std::thread(&WindowManager::main_th, &wm, 615, 462));
  }
  threads.emplace_back(std::thread(&CameraController::capture, &cam));
  while (1) {
    /*
    if (cam.ready()) {
      printf("CAM\n");
    }*/
    /*
    if (wm.ready()) {
      printf("WM\n");
    }
    */
    if (cam.ready()) {
      if (window_enabled && !wm.ready()) {
        continue;
      }

      break;
    }
    /*
     if (cam.ready()) {
       break;
     }
     */
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
  threads.emplace_back(std::thread(calc));
  threads.emplace_back(std::thread(&WindowManager::update_th, &wm, &fb));

  pthread_sigmask(SIG_UNBLOCK, &mask, NULL); // スレッドを作ったので割り込み許可

  printf("READY\n");

  // wait_start();
  wait_start_btn();
  printf("START\n");

  run();
  stop();

  for (auto &th : threads) {
    th.join();
  }
}

void setup_scenario() {
  runner = new ScenarioRunner(*config->getValueArray("scenarios"), left, right,
                              &line_monitor);
}

void calc() {
  // printf("calc\n");
  int id = -1;
  if (window_enabled) {
    id = wm.create_window(600, 400, "roi");
  }

  while (true) {
    cv::Mat frame = cam.getFrame();
    if (frame.empty()) {
      continue;
    }
    line_monitor.update(frame, &fb);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000 / 30));
  }
}

void wait_start() {
  while (1) {
    int distance = pup_ultrasonic_sensor_distance(ultrasonic_sensor);
    // printf("distance: %d\n", distance);
    if (500 > distance && distance > 50) {
      break;
    }
    tslp_tsk(10 * 1000);
  }
}

void wait_start_btn() {
  while (1) {
    hub_button_t pressed;
    hub_button_is_pressed(&pressed);
    if (pressed == HUB_BUTTON_BT) {
      break;
    }
    tslp_tsk(10 * 1000);
  }
}

void setup_motor() {
  left = pup_motor_get_device(LEFT_MOTOR_PORT);
  right = pup_motor_get_device(RIGHT_MOTOR_PORT);

  if (left == NULL || right == NULL) {
    printf("motor not found\n");
    ext_tsk();
  }

  pup_motor_setup(left, LEFT_MOTOR_DIRECTION, true);
  pup_motor_setup(right, RIGHT_MOTOR_DIRECTION, true);

  pup_motor_set_duty_limit(left, 100);
  pup_motor_set_duty_limit(right, 100);
}

void setup_ultrasonic_sensor() {
  ultrasonic_sensor = pup_ultrasonic_sensor_get_device(PBIO_PORT_ID_F);
  if (ultrasonic_sensor == NULL) {
    printf("ultrasonic sensor not found\n");
    ext_tsk();
  }
}

void run() {
  while (1) {
    if (runner->run()) {
      break;
    }
    tslp_tsk(10 * 1000);
  }
}

void stop() {
  pup_motor_stop(left);
  pup_motor_stop(right);
}

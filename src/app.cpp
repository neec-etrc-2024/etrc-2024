#include "app.hpp"
#include "devices/CameraController.hpp"
#include "gui/WindowManager.hpp"
#include "informations/CameraLineMonitor.hpp"
#include "raspike_port.h"
#include <FL/Fl.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_RGB_Image.H>
#include <FL/Fl_Window.H>
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
}

void calc();

void test1();

void setup_motor();
void wait_start();

WindowManager wm;
CameraController cam;
informations::CameraLineMonitor line_monitor;

pup_motor_t *left = NULL;
pup_motor_t *right = NULL;

void main_task(intptr_t unused) { // main_task 最初に呼ばれる
  pup_motor_t *left = NULL;
  // left = pup_motor_get_device(PBIO_PORT_ID_E);
  pup_motor_t *right = NULL;
  // right = pup_motor_get_device(PBIO_PORT_ID_B);
  pup_device_t *colorsensor = NULL;
  // colorsensor = pup_color_sensor_get_device(PBIO_PORT_ID_C);

  if (left == NULL || right == NULL || colorsensor == NULL) {
    printf("motor not found\n");
    // return;
  }

  // pup_motor_setup(left, PUP_DIRECTION_COUNTERCLOCKWISE, true);
  // pup_motor_setup(right, PUP_DIRECTION_CLOCKWISE, true);

  // pup_motor_set_duty_limit(left, 100);
  // pup_motor_set_duty_limit(right, 100);
  // printf("duty: %d\n", duty);

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
  threads.emplace_back(std::thread(&WindowManager::main_th, &wm, 615, 462));
  threads.emplace_back(std::thread(&CameraController::capture, &cam));
  while (1) {
    /*
    if (cam.ready()) {
      printf("CAM\n");
    }*/
    if (wm.ready()) {
      printf("WM\n");
    }
    if (cam.ready() && wm.ready()) {
      break;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
  threads.emplace_back(std::thread(calc));

  pthread_sigmask(SIG_UNBLOCK, &mask, NULL); // スレッドを作ったので割り込み許可

  printf("READY\n");

  int duration = 1000 * 1000 / 30; // 30 fps
  wait_start();
  printf("START\n");

  test1();

  while (1) {
    double diff2 = line_monitor.get_differences();
    printf("diff2: %f\n", diff2);
    int base_speed = 50;

    //  printf("diff: %d\n", diff);

    if (diff2 < 0) {
      pup_motor_set_power(right, base_speed);
      pup_motor_set_power(left, base_speed - 20);
    } else {
      pup_motor_set_power(right, base_speed - 20);
      pup_motor_set_power(left, base_speed);
    }

    tslp_tsk(1000 * 1000 / 30);
  }

  printf("start\n");
  /*
    for (auto &th : threads) {
      th.join();
    }
    */
}

void calc() {
  // printf("calc\n");
  int id = wm.create_window(600, 400, "roi");

  while (true) {
    cv::Mat frame = cam.getFrame();
    if (frame.empty()) {
      continue;
    }
    line_monitor.update(frame, id);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000 / 30));
  }
}

void test1() {
  while (1) {
    tslp_tsk(10 * 1000);
  }
}

void wait_start() {
  while (1) {
    hub_button_t pressed;
    hub_button_is_pressed(&pressed);
    if (pressed == HUB_BUTTON_LEFT) {
      break;
    }
    tslp_tsk(10 * 1000);
  }
}

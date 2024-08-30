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
void pid_run();
void curve();
void stop();

WindowManager wm;
CameraController cam;
informations::CameraLineMonitor line_monitor;

pup_motor_t *left = NULL;
pup_motor_t *right = NULL;

void main_task(intptr_t unused) { // main_task 最初に呼ばれる

  setup_motor();

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

  // pid_run();
  curve();
  stop();

  for (auto &th : threads) {
    th.join();
  }
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

void pid_run() {
  FILE *fp = fopen("motor.conf", "r");
  if (fp == NULL) {
    printf("motor.conf not found\n");
    ext_tsk();
  }

  int base_power = 50;
  int kp = 20;
  int kd = 5;

  fscanf(fp, "%d %d %d", &base_power, &kp, &kd);

  fclose(fp);

  int duration = 1000 * 1000 / 30; // 30 fps
  double prev_diff = 100;
  line_monitor.set_trace_left(false);
  while (1) {
    double diff = line_monitor.get_differences();
    if (prev_diff > 1) {
      prev_diff = diff;
    }

    double diff_diff = diff - prev_diff;

    int left_power, right_power;
    if (diff >= 0) {
      right_power = base_power - diff * kp;
      left_power = base_power;
    } else {
      left_power = base_power + diff * kp;
      right_power = base_power;
    }

    if (diff_diff >= 0) {
      right_power -= diff_diff * kd;
    } else {
      left_power += diff_diff * kd;
    }

    pup_motor_set_power(left, left_power);
    pup_motor_set_power(right, right_power);

    int left_count = pup_motor_get_count(left);
    int right_count = pup_motor_get_count(right);

    if (left_count >= 2900 && right_count >= 2900) {
      break;
    }

    prev_diff = diff;
    tslp_tsk(duration);
  }
}

void curve() {
  FILE *fp = fopen("curve.conf", "r");
  if (fp == NULL) {
    printf("motor.conf not found\n");
    ext_tsk();
  }

  int base_power = 60;
  int kp = 20;
  double ki = 0;
  int kd = 5;
  double sum = 0;

  fscanf(fp, "%d %d %lf %d", &base_power, &kp, &ki, &kd);

  fclose(fp);

  int duration = 1000 * 1000 / 30; // 30 fps
  double prev_diff = 100;
  line_monitor.set_trace_left(false);
  while (1) {
    double diff = line_monitor.get_differences();
    sum += diff;
    if (prev_diff > 1) {
      prev_diff = diff;
    }

    double diff_diff = diff - prev_diff;

    int left_power, right_power;
    if (diff >= 0) {
      right_power = base_power - diff * kp;
      left_power = base_power;
    } else {
      left_power = base_power + diff * kp;
      right_power = base_power;
    }

    if (diff_diff >= 0) {
      right_power -= diff_diff * kd;
    } else {
      left_power += diff_diff * kd;
    }

    printf("sum*ki=%lf\n", sum * ki);
    if (sum >= 0) {
      right_power -= sum * ki;
    } else {
      left_power += sum * ki;
    }

    printf("left:%d,right:%d\n", left_power, right_power);

    pup_motor_set_power(left, left_power);
    pup_motor_set_power(right, right_power);

    int left_count = pup_motor_get_count(left);
    int right_count = pup_motor_get_count(right);

    /*
    if (left_count >= 2900 && right_count >= 2900) {
      break;
    }
    */

    prev_diff = diff;
    tslp_tsk(duration);
  }
}

void stop() {
  pup_motor_stop(left);
  pup_motor_stop(right);
}
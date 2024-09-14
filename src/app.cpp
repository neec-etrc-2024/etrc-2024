#include "app.hpp"
#include "JsonParser.hpp"
#include "Pid.hpp"
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

extern "C"
{
#include "spike/hub/button.h"
#include "spike/pup/colorsensor.h"
#include "spike/pup/motor.h"
#include "spike/pup/ultrasonicsensor.h"
}

void calc();

void test1();

void setup_motor();
void setup_ultrasonic_sensor();
void wait_start();
void straight1();
void curve1();
void curve2();
void straight2();
void pid_run();
void kukan0();
void kukan1();
void kukan2();
void kukan3();
void stop();
void setup_pid();

WindowManager wm;
CameraController cam;
informations::CameraLineMonitor line_monitor;

pup_motor_t *left = NULL;
pup_motor_t *right = NULL;
pup_device_t *ultrasonic_sensor = NULL;

bool window_enabled = false;
bool straight1_enable = false;
bool straight2_enable = false;
bool curve1_enable = false;
bool curve2_enable = false;
bool kukan0_enable = false;
bool kukan1_enable = false;
bool kukan2_enable = false;
bool kukan3_enable = false;
int straight1_end = 0;
int straight2_end = 0;
int kukan0_end = 0;
int kukan1_end = 0;
int kukan2_end = 0;
int kukan3_end = 0;
int curve1_end = 0;
int curve2_end = 0;

Pid *straight_pid = nullptr;
Pid *kukan1_pid = nullptr;
Pid *curve_pid = nullptr;

void main_task(intptr_t unused)
{ // main_task 最初に呼ばれる

  loadConfig();

  setup_motor();
  setup_ultrasonic_sensor();
  setup_pid();

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
  if (window_enabled)
  {
    threads.emplace_back(std::thread(&WindowManager::main_th, &wm, 615, 462));
  }
  threads.emplace_back(std::thread(&CameraController::capture, &cam));
  while (1)
  {
    /*
    if (cam.ready()) {
      printf("CAM\n");
    }*/
    /*
    if (wm.ready()) {
      printf("WM\n");
    }
    */
    if (cam.ready())
    {
      if (window_enabled && !wm.ready())
      {
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

  pthread_sigmask(SIG_UNBLOCK, &mask, NULL); // スレッドを作ったので割り込み許可

  printf("READY\n");

  int duration = 1000 * 1000 / 30; // 30 fps
  wait_start();
  printf("START\n");

  straight1();
  curve1();
  straight2();
  curve2();
  kukan0();
  kukan1();
  kukan2();
  kukan3();
  // curve();
  stop();

  for (auto &th : threads)
  {
    th.join();
  }
}

void calc()
{
  // printf("calc\n");
  int id = -1;
  if (window_enabled)
  {
    id = wm.create_window(600, 400, "roi");
  }

  while (true)
  {
    cv::Mat frame = cam.getFrame();
    if (frame.empty())
    {
      continue;
    }
    line_monitor.update(frame, id);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000 / 30));
  }
}

void test1()
{
  while (1)
  {
    tslp_tsk(10 * 1000);
  }
}

void wait_start()
{
  while (1)
  {
    int distance = pup_ultrasonic_sensor_distance(ultrasonic_sensor);
    // printf("distance: %d\n", distance);
    if (distance > 50)
    {
      break;
    }
    tslp_tsk(10 * 1000);
  }
}

void setup_motor()
{
  left = pup_motor_get_device(LEFT_MOTOR_PORT);
  right = pup_motor_get_device(RIGHT_MOTOR_PORT);

  if (left == NULL || right == NULL)
  {
    printf("motor not found\n");
    ext_tsk();
  }

  pup_motor_setup(left, LEFT_MOTOR_DIRECTION, true);
  pup_motor_setup(right, RIGHT_MOTOR_DIRECTION, true);

  pup_motor_set_duty_limit(left, 100);
  pup_motor_set_duty_limit(right, 100);
}

void setup_ultrasonic_sensor()
{
  ultrasonic_sensor = pup_ultrasonic_sensor_get_device(PBIO_PORT_ID_F);
  if (ultrasonic_sensor == NULL)
  {
    printf("ultrasonic sensor not found\n");
    ext_tsk();
  }
}

void setup_pid()
{
  window_enabled = config["window"];
  int base_power = config["straight1"]["base_speed"];
  int kp = config["straight1"]["p"];
  double ki = config["straight1"]["i"];
  int kd = config["straight1"]["d"];

  straight_pid = new Pid(left, right, LEFT, kp, ki, kd, base_power);

  base_power = config["curve1"]["base_speed"];
  kp = config["curve1"]["p"];
  ki = config["curve1"]["i"];
  kd = config["curve1"]["d"];

  straight1_enable = config["straight1"]["enable"];
  straight2_enable = config["straight2"]["enable"];
  curve1_enable = config["curve1"]["enable"];
  curve2_enable = config["curve2"]["enable"];
  kukan0_enable = config["kukan0"]["enable"];
  kukan1_enable = config["kukan1"]["enable"];
  kukan2_enable = config["kukan2"]["enable"];
  kukan3_enable = config["kukan3"]["enable"];

  straight1_end = config["straight1"]["end"];
  straight2_end = config["straight2"]["end"];
  curve1_end = config["curve1"]["end"];
  curve2_end = config["curve2"]["end"];
  kukan0_end = config["kukan0"]["end"];
  kukan1_end = config["kukan1"]["end"];
  kukan2_end = config["kukan2"]["end"];
  kukan3_end = config["kukan3"]["end"];

  curve_pid = new Pid(left, right, LEFT, kp, ki, kd, base_power);

  base_power = config["kukan1"]["base_speed"];
  kp = config["kukan1"]["p"];
  ki = config["kukan1"]["i"];
  kd = config["kukan1"]["d"];

  kukan1_pid = new Pid(left, right, LEFT, kp, ki, kd, base_power);
}

void straight1()
{
  printf("straight1\n");
  if (!straight1_enable)
  {
    return;
  }
  int duration = 1000 * 1000 / 30; // 30 fps
  while (1)
  {
    double diff = line_monitor.get_differences();
    straight_pid->run(diff);

    int left_count = pup_motor_get_count(left);
    int right_count = pup_motor_get_count(right);

    // printf("left:%d,right:%d\n", left_count, right_count);
    // int wid = line_monitor.get_line_width();
    // printf("width:%d\n", wid);
    // int blue = line_monitor.get_blue_count();
    // printf("blue:%d\n", blue);

    if (left_count >= straight1_end && right_count >= straight1_end)
    {
      break;
    }
    tslp_tsk(duration);
  }
}

void curve1()
{
  printf("curve\n");
  if (!curve1_enable)
  {
    return;
  }
  int duration = 1000 * 1000 / 30; // 30 fps
  pup_motor_reset_count(left);
  pup_motor_reset_count(right);
  while (1)
  {
    double diff = line_monitor.get_differences();
    curve_pid->run(diff);

    int left_count = pup_motor_get_count(left);
    int right_count = pup_motor_get_count(right);

    printf("left:%d,right:%d\n", left_count, right_count);
    // int wid = line_monitor.get_line_width();
    // printf("width:%d\n", wid);
    // int blue = line_monitor.get_blue_count();
    // printf("blue:%d\n", blue);

    if (left_count >= curve1_end && right_count >= curve1_end)
    {
      break;
    }
    tslp_tsk(duration);
  }
}

void straight2()
{
  printf("straight2\n");
  if (!straight2_enable)
  {
    return;
  }
  int duration = 1000 * 1000 / 30; // 30 fps
  pup_motor_reset_count(left);
  pup_motor_reset_count(right);
  while (1)
  {
    double diff = line_monitor.get_differences();
    straight_pid->run(diff);

    int left_count = pup_motor_get_count(left);
    int right_count = pup_motor_get_count(right);

    // printf("left:%d,right:%d\n", left_count, right_count);
    // int wid = line_monitor.get_line_width();
    // printf("width:%d\n", wid);
    // int blue = line_monitor.get_blue_count();
    // printf("blue:%d\n", blue);

    if (left_count >= straight2_end && right_count >= straight2_end)
    {
      break;
    }
    tslp_tsk(duration);
  }
}

void curve2()
{
  if (!curve2_enable)
  {
    return;
  }

  int duration = 1000 * 1000 / 30; // 30 fps
  pup_motor_reset_count(left);
  pup_motor_reset_count(right);
  while (1)
  {
    double diff = line_monitor.get_differences();

    curve_pid->run(diff);

    int left_count = pup_motor_get_count(left);
    int right_count = pup_motor_get_count(right);
    if (left_count >= curve2_end && right_count >= curve2_end)
    {
      break;
    }
    tslp_tsk(duration);
  }
}

void kukan0()
{
  printf("kukan0\n");
  if (!kukan0_enable)
  {
    return;
  }

  int duration = 1000 * 1000 / 30; // 30 fps
  pup_motor_reset_count(left);
  pup_motor_reset_count(right);
  while (1)
  {
    double diff = line_monitor.get_differences();
    kukan1_pid->run(diff);

    int left_count = pup_motor_get_count(left);
    int right_count = pup_motor_get_count(right);

    // printf("left:%d,right:%d\n", left_count, right_count);
    // printf("blue:%d\n", blue);
    if (left_count >= kukan0_end && right_count >= kukan0_end)
    {
      break;
    }
    tslp_tsk(duration);
  }
}

void kukan1()
{
  printf("kukan1\n");
  if (!kukan1_enable)
  {
    return;
  }

  int duration = 1000 * 1000 / 30; // 30 fps
  while (1)
  {
    double diff = line_monitor.get_differences();
    kukan1_pid->run(diff);

    int left_count = pup_motor_get_count(left);
    int right_count = pup_motor_get_count(right);

    // printf("left:%d,right:%d\n", left_count, right_count);
    int blue = line_monitor.get_blue_count();
    // printf("blue:%d\n", blue);

    if (blue < kukan1_end)
    {
      break;
    }
    tslp_tsk(duration);
  }
}

void kukan2()
{
  printf("kukan2\n");
  if (!kukan2_enable)
  {
    return;
  }

  line_monitor.set_trace_left(false);

  int duration = 1000 * 1000 / 30; // 30 fps
  while (1)
  {
    double diff = line_monitor.get_differences();
    kukan1_pid->run(diff);

    int left_count = pup_motor_get_count(left);
    int right_count = pup_motor_get_count(right);

    // printf("left:%d,right:%d\n", left_count, right_count);
    // int wid = line_monitor.get_line_width();
    // printf("width:%d\n", wid);

    int blue = line_monitor.get_blue_count();
    // printf("blue:%d\n", blue);

    if (blue >= kukan2_end)
    {
      break;
    }

    tslp_tsk(duration);
  }
}

void kukan3()
{
  printf("kukan3\n");
  if (!kukan3_enable)
  {
    return;
  }

  line_monitor.set_trace_left(true);

  int duration = 1000 * 1000 / 30; // 30 fps
  while (1)
  {
    double diff = line_monitor.get_differences();
    kukan1_pid->run(diff);

    int left_count = pup_motor_get_count(left);
    int right_count = pup_motor_get_count(right);

    // printf("left:%d,right:%d\n", left_count, right_count);
    int wid = line_monitor.get_line_width();
    printf("width:%d\n", wid);

    int blue = line_monitor.get_blue_count();
    // printf("blue:%d\n", blue);

    if (blue >= kukan3_end)
    {
      break;
    }

    tslp_tsk(duration);
  }
}

/*
void pid_run() {
  printf("pid_run\n");

  if (!straight_enable) {
    return;
  }

  // line_monitor.set_trace_left(false);

  int duration = 1000 * 1000 / 30; // 30 fps
  while (1) {
    double diff = line_monitor.get_differences();
    straight_pid->run(diff);

    int left_count = pup_motor_get_count(left);
    int right_count = pup_motor_get_count(right);

    // printf("left:%d,right:%d\n", left_count, right_count);
    // int wid = line_monitor.get_line_width();
    // printf("width:%d\n", wid);
    int blue = line_monitor.get_blue_count();
    // printf("blue:%d\n", blue);

    if (blue >= straight_end) {
      break;
    }
    tslp_tsk(duration);
  }
}
*/

void stop()
{
  pup_motor_stop(left);
  pup_motor_stop(right);
}

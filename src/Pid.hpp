#ifndef _PID_HPP_
#define _PID_HPP_

extern "C" {
#include "spike/hub/button.h"
#include "spike/pup/colorsensor.h"
#include "spike/pup/motor.h"
#include "spike/pup/ultrasonicsensor.h"
}

enum DIRECTION { LEFT, RIGHT };

class Pid {
private:
  pup_motor_t *left = nullptr;
  pup_motor_t *right = nullptr;
  DIRECTION direction;
  int base_power;
  double kp;
  double ki;
  double kd;
  double prev_diff;
  double sum_diff;

public:
  Pid(pup_motor_t *left, pup_motor_t *right, DIRECTION direction, double kp,
      double ki, double kd, int base_power)
      : left(left), right(right), direction(direction), kp(kp), ki(ki), kd(kd),
        prev_diff(0), sum_diff(0), base_power(base_power) {};

  void run(double diff);
  ~Pid() {};
};

#endif // _PID_HPP_
#include "Pid.hpp"

#include <stdio.h>

void Pid::run(double diff) {
  sum_diff += diff;
  double p = kp * diff;
  double i = ki * sum_diff;
  double d = kd * (diff - prev_diff);
  double power = p + i + d;
  prev_diff = diff;

  // printf("p:%f,i:%f,d:%f,power:%f\n", p, i, d, power);
  if (direction == LEFT) {
    pup_motor_set_power(left, base_power + power);
    pup_motor_set_power(right, base_power - power);
  } else {
    pup_motor_set_power(left, base_power - power);
    pup_motor_set_power(right, base_power + power);
  }
}
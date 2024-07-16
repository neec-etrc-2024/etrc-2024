#ifndef _RUN_MOTOR_CONTROLLER_HPP_
#define _RUN_MOTOR_CONTROLLER_HPP_

#include "Motor.hpp"

using namespace spikeapi;

namespace devices {

struct MotorCounts {
  int left;
  int right;
};

class RunMotorController {
private:
  Motor &left_wheel;
  Motor &right_wheel;

public:
  RunMotorController(Motor &left_wheel, Motor &right_wheel);
  void set_pwm(int left_pwm, int right_pwm);
  void get_counts(MotorCounts &counts);
  void update();
  void apply_pwm();
  void init();
  ~RunMotorController();
};
} // namespace devices

#endif // _RUN_MOTOR_CONTROLLER_HPP_
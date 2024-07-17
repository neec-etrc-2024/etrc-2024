#ifndef _RUN_MOTOR_CONTROLLER_HPP_
#define _RUN_MOTOR_CONTROLLER_HPP_

#include "Motor.hpp"
#include <mutex>

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

  int left_pwm;
  int right_pwm;
  std::mutex pwm_mutex;

  MotorCounts counts;
  std::mutex counts_mutex;

  int check_pwm(int pwm);

public:
  RunMotorController(Motor &left_wheel, Motor &right_wheel);
  void set_pwm(int left_pwm, int right_pwm);
  void get_counts(MotorCounts &counts);
  void update();
  void apply_pwm();
  void init();
  ~RunMotorController();
};

RunMotorController::RunMotorController(Motor &left_wheel, Motor &right_wheel)
    : left_wheel(left_wheel), right_wheel(right_wheel), left_pwm(0),
      right_pwm(0), pwm_mutex(), counts(), counts_mutex() {}

RunMotorController::~RunMotorController() {}
} // namespace devices

#endif // _RUN_MOTOR_CONTROLLER_HPP_
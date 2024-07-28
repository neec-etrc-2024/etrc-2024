#ifndef _RUN_MOTOR_CONTROLLER_HPP_
#define _RUN_MOTOR_CONTROLLER_HPP_

#include "Motor.hpp"
extern "C" {
#include "kernel.h"
}

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
  ID powersem_id;

  MotorCounts counts;
  ID countssem_id;

  int check_pwm(int pwm);

public:
  RunMotorController(Motor &left_wheel, Motor &right_wheel, ID powersem_id,
                     ID countssem_id);
  void set_pwm(int left_pwm, int right_pwm);
  void get_counts(MotorCounts &counts);
  void update();
  void apply_pwm();
  void init();
  ~RunMotorController();
};

inline RunMotorController::RunMotorController(Motor &left_wheel,
                                              Motor &right_wheel,
                                              ID powersem_id, ID countssem_id)
    : left_wheel(left_wheel), right_wheel(right_wheel), left_pwm(0),
      right_pwm(0), powersem_id(powersem_id), counts(),
      countssem_id(countssem_id) {}

inline RunMotorController::~RunMotorController() {}
} // namespace devices

#endif // _RUN_MOTOR_CONTROLLER_HPP_
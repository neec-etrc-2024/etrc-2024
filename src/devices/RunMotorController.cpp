#include "devices/RunMotorController.hpp"
#include <cmath>

namespace devices {

void RunMotorController::set_pwm(int left_pwm, int right_pwm) {
  left_pwm = check_pwm(left_pwm);
  right_pwm = check_pwm(right_pwm);

  wai_sem(this->powersem_id);
  this->left_pwm = left_pwm;
  this->right_pwm = right_pwm;
  sig_sem(this->powersem_id);
}

int RunMotorController::check_pwm(int pwm) {
  if (pwm > 100) {
    return 100;
  } else if (pwm < -100) {
    return -100;
  } else {
    return pwm;
  }
}

void RunMotorController::get_counts(MotorCounts &counts) {
  wai_sem(this->countssem_id);
  counts = this->counts;
  sig_sem(this->countssem_id);
}

void RunMotorController::update() {
  MotorCounts counts_buf;
  counts_buf.left = left_wheel.get_count();
  counts_buf.right = right_wheel.get_count();

  wai_sem(this->countssem_id);
  counts = counts_buf;
  sig_sem(this->countssem_id);
}

void RunMotorController::apply_pwm() {
  wai_sem(this->powersem_id);
  left_wheel.set_power(left_pwm);
  right_wheel.set_power(right_pwm);
  sig_sem(this->powersem_id);
}

void RunMotorController::init() {
  left_wheel.stop();
  right_wheel.stop();
  left_wheel.reset_count();
  right_wheel.reset_count();
}
} // namespace devices
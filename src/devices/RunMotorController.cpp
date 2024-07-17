#include "devices/RunMotorController.hpp"
#include <cmath>

using namespace devices;

void RunMotorController::set_pwm(int left_pwm, int right_pwm) {
  left_pwm = check_pwm(left_pwm);
  right_pwm = check_pwm(right_pwm);

  pwm_mutex.lock();
  this->left_pwm = left_pwm;
  this->right_pwm = right_pwm;
  pwm_mutex.unlock();
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
  counts_mutex.lock();
  counts = this->counts;
  counts_mutex.unlock();
}

void RunMotorController::update() {
  MotorCounts counts_buf;
  counts_buf.left = left_wheel.get_count();
  counts_buf.right = right_wheel.get_count();

  counts_mutex.lock();
  counts = counts_buf;
  counts_mutex.unlock();
}

void RunMotorController::apply_pwm() {
  pwm_mutex.lock();
  left_wheel.set_power(left_pwm);
  right_wheel.set_power(right_pwm);
  pwm_mutex.unlock();
}

void RunMotorController::init() {
  left_wheel.stop();
  right_wheel.stop();
  left_wheel.reset_count();
  right_wheel.reset_count();
}
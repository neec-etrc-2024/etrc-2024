#include "behaviors/LineTrace.hpp"

namespace behaviors {

void LineTrace::init() {
  // motor_controller.init();
}

void LineTrace::run() {
  double diff = this->line_monitor.get_differences();
  double pid = this->calc_pid(diff);

  int pwm = this->base_speed * pid; // 減じるPWM値
  int right_pwm = this->base_speed;
  int left_pwm = this->base_speed;

  switch (line_direction) {
  case LEFT:       // 左LT(線の左に機体がある)
    if (pwm < 0) { // 線に近い -> 左に曲がる(左を減速)
      left_pwm += pwm;
    } else { // 線から遠い -> 右に曲がる(右を減速)
      right_pwm -= pwm;
    }
    break;
  case RIGHT:      // 右LT(線の右に機体がある)
    if (pwm < 0) { // 線に近い -> 右に曲がる(右を減速)
      right_pwm += pwm;
    } else { // 線から遠い -> 左に曲がる(左を減速)
      left_pwm -= pwm;
    }
    break;
  default:
    break;
  }

  this->motor_controller.set_pwm(left_pwm, right_pwm);
}

double LineTrace::calc_pid(double diff) { return kp * diff; }

} // namespace behaviors
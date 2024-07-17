#include "behaviors/Straight.hpp"

using namespace behaviors;

void Straight::run() {
  this->motor_controller.set_pwm(this->base_speed, this->base_speed);
}

void Straight::init() { this->motor_controller.init(); }

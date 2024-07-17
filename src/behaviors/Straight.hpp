#ifndef _STRAIGHT_HPP_
#define _STRAIGHT_HPP_

#include "behaviors/IBehavior.hpp"
#include "devices/RunMotorController.hpp"

using namespace devices;

namespace behaviors {
class Straight : public IBehavior {
private:
  int base_speed;
  RunMotorController &motor_controller;

public:
  Straight(RunMotorController &motor_controller, int base_speed = 50);
  void run() override;
  void init() override;
  ~Straight();
};

Straight::Straight(RunMotorController &motor_controller, int base_speed = 50)
    : base_speed(base_speed), motor_controller(motor_controller) {}

Straight::~Straight() {}

} // namespace behaviors

#endif // _STRAIGHT_HPP_
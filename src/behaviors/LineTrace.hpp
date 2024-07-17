#ifndef _LINE_TRACE_HPP_
#define _LINE_TRACE_HPP_

#include "behaviors/IBehavior.hpp"
#include "devices/RunMotorController.hpp"
#include "informations/ILineMonitor.hpp"

using namespace devices;
using namespace informations;

namespace behaviors {

enum LINE_DIRECTION { // 線を左右どちらに見るか
  LEFT,
  RIGHT,
};

class LineTrace : public IBehavior {
private:
  LINE_DIRECTION line_direction;
  RunMotorController &motor_controller;
  ILineMonitor &line_monitor;
  int base_speed;
  double kp;
  double ki;
  double kd;

public:
  LineTrace(LINE_DIRECTION line_direction, RunMotorController &motor_controller,
            ILineMonitor &line_monitor, int base_speed = 50, double kp = 0.0,
            double ki = 0.0, double kd = 0.0);
  void run() override;
  void init() override;
  ~LineTrace();
};

LineTrace::LineTrace(LINE_DIRECTION line_direction,
                     RunMotorController &motor_controller,
                     ILineMonitor &line_monitor, int base_speed = 50,
                     double kp = 0.0, double ki = 0.0, double kd = 0.0)
    : line_direction(line_direction), motor_controller(motor_controller),
      line_monitor(line_monitor), base_speed(base_speed), kp(kp), ki(ki),
      kd(kd) {}

LineTrace::~LineTrace() {}

} // namespace behaviors

#endif // _LINE_TRACE_HPP_
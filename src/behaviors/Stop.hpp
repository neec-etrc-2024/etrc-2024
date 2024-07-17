#ifndef _STOP_HPP_
#define _STOP_HPP_

#include "behaviors/IBehavior.hpp"
#include "devices/RunMotorController.hpp"

using namespace devices;

namespace behaviors {
class Stop : public IBehavior {
private:
  RunMotorController &motorController;

public:
  Stop(RunMotorController &motorController);
  void run() override;
  void init() override;
};

Stop::Stop(RunMotorController &motorController)
    : IBehavior(), motorController(motorController) {}
} // namespace behaviors

#endif // _STOP_HPP_
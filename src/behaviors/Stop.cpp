#include "behaviors/Stop.hpp"

using namespace behaviors;

void Stop::run() { motorController.set_pwm(0, 0); }

void Stop::init() { motorController.init(); }
#include "conditions/ButtonCondition.hpp"

using namespace conditions;

bool ButtonCondition::isCompleted() { return hub.is_button_pressed(button); }

void ButtonCondition::init() {}
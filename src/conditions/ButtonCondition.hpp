#ifndef _BUTTON_CONDITION_HPP_
#define _BUTTON_CONDITION_HPP_

#include "Hub.hpp"
#include "conditions/ICondition.hpp"

namespace conditions {
class ButtonCondition : public ICondition {
private:
  hub_button_t button; // 検知対象のボタン
  spikeapi::Hub &hub;

public:
  ButtonCondition(hub_button_t button, spikeapi::Hub &hub);
  bool isCompleted() override;
  void init() override;
  ~ButtonCondition();
};

inline ButtonCondition::ButtonCondition(hub_button_t button, spikeapi::Hub &hub)
    : button(button), hub(hub) {}

inline ButtonCondition::~ButtonCondition() {}

} // namespace conditions

#endif // _BUTTON_CONDITION_HPP_
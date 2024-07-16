#ifndef _BUTTON_CONDITION_HPP_
#define _BUTTON_CONDITION_HPP_

#include "conditions/ICondition.hpp"

namespace conditions {
class ButtonCondition : public ICondition {
private:
  /* DATA */
public:
  ButtonCondition(/* args */);
  bool isCompleted() override;
  void init() override;
  ~ButtonCondition();
};
} // namespace conditions

#endif // _BUTTON_CONDITION_HPP_
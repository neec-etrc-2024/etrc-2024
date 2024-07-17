#ifndef _DISTANCE_CONDITION_HPP_
#define _DISTANCE_CONDITION_HPP_

#include "conditions/ICondition.hpp"

namespace conditions {
class DistanceCondition : public ICondition {
private:
  /* data */
public:
  DistanceCondition(/* args */);
  bool isCompleted() override;
  void init() override;
  ~DistanceCondition();
};

DistanceCondition::DistanceCondition(/* args */) {}

bool DistanceCondition::isCompleted() { return false; }
void DistanceCondition::init() {}

DistanceCondition::~DistanceCondition() {}
} // namespace conditions

#endif // _DISTANCE_CONDITION_HPP_
#ifndef _STRAIGHT_HPP_
#define _STRAIGHT_HPP_

#include "behaviors/IBehavior.hpp"

namespace behaviors {
class Straight : public IBehavior {
private:
  /* data */
public:
  Straight(/* args */);
  void run() override;
  void init() override;
  ~Straight();
};

Straight::Straight(/* args */) {}

Straight::~Straight() {}

} // namespace behaviors

#endif // _STRAIGHT_HPP_
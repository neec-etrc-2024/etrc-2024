#ifndef _STOP_HPP_
#define _STOP_HPP_

#include "behaviors/IBehavior.hpp"

namespace behaviors {
class Stop : public IBehavior {
private:
  /* data */
public:
  Stop(/* args */);
  void run() override;
  void init() override;
  ~Stop();
};
} // namespace behaviors

#endif // _STOP_HPP_
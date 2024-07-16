#ifndef _I_BEHAVIOR_HPP_
#define _I_BEHAVIOR_HPP_

namespace behaviors {
class IBehavior {
public:
  virtual void run() = 0;
  virtual void init() = 0;
};

} // namespace behaviors

#endif // _I_BEHAVIOR_HPP_
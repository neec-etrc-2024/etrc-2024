#ifndef _ICONDITION_HPP_

#define _ICONDITION_HPP_

namespace conditions {
class ICondition {
public:
  virtual bool isCompleted() = 0;
  virtual void init() = 0;
};

} // namespace conditions

#endif // _ICONDITION_HPP_
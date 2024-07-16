#ifndef _LINE_TRACE_HPP_
#define _LINE_TRACE_HPP_

#include "behaviors/IBehavior.hpp"

namespace behaviors {
class LineTrace : public IBehavior {
private:
  /* data */
public:
  LineTrace(/* args */);
  void run() override;
  void init() override;
  ~LineTrace();
};

LineTrace::LineTrace(/* args */) {}

LineTrace::~LineTrace() {}

} // namespace behaviors

#endif // _LINE_TRACE_HPP_
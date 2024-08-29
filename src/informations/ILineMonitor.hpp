#ifndef _ILINE_MONITOR_HPP_
#define _ILINE_MONITOR_HPP_

namespace informations {
class ILineMonitor {
public:
  virtual double get_differences() = 0;
};
} // namespace informations

#endif // _ILINE_MONITOR_HPP_
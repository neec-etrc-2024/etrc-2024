#ifndef _CAMERA_LINE_MONITOR_HPP_
#define _CAMERA_LINE_MONITOR_HPP_

#include <informations/ILineMonitor.hpp>

namespace informations {
class CameraLineMonitor : public ILineMonitor {
private:
  /* data */
public:
  CameraLineMonitor(/* args */);
  double get_differences() const override;
  ~CameraLineMonitor();
};

inline CameraLineMonitor::CameraLineMonitor(/* args */) {}

inline CameraLineMonitor::~CameraLineMonitor() {}

inline double CameraLineMonitor::get_differences() const { return 0; }

} // namespace informations

#endif // _CAMERA_LINE_MONITOR_HPP_
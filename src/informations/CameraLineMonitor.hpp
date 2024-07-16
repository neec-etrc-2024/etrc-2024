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

CameraLineMonitor::CameraLineMonitor(/* args */) {}

CameraLineMonitor::~CameraLineMonitor() {}

} // namespace informations

#endif // _CAMERA_LINE_MONITOR_HPP_
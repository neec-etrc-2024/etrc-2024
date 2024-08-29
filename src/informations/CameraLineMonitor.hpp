#ifndef _CAMERA_LINE_MONITOR_HPP_
#define _CAMERA_LINE_MONITOR_HPP_

#include <informations/ILineMonitor.hpp>
#include <mutex>
#include <opencv2/opencv.hpp>

namespace informations {
class CameraLineMonitor : public ILineMonitor {
private:
  volatile double diff;
  std::mutex mtx;
  volatile bool trace_left;

public:
  CameraLineMonitor();
  double get_differences() override;
  void update(cv::Mat &img);
  ~CameraLineMonitor();
};

inline CameraLineMonitor::CameraLineMonitor()
    : diff(), mtx(), trace_left(true) {}

inline CameraLineMonitor::~CameraLineMonitor() {}

inline double CameraLineMonitor::get_differences() {
  mtx.lock();
  double ret = diff;
  mtx.unlock();
  return ret;
}

} // namespace informations

#endif // _CAMERA_LINE_MONITOR_HPP_
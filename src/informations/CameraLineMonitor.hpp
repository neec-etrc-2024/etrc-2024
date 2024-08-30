#ifndef _CAMERA_LINE_MONITOR_HPP_
#define _CAMERA_LINE_MONITOR_HPP_

#include <atomic>
#include <informations/ILineMonitor.hpp>
#include <mutex>
#include <opencv2/opencv.hpp>

namespace informations {
class CameraLineMonitor : public ILineMonitor {
private:
  double diff;
  std::mutex mtx;
  std::atomic_bool trace_left;
  std::atomic_int line_width;

public:
  CameraLineMonitor();
  double get_differences() override;
  void update(cv::Mat &img, int window_id = -1);
  int get_line_width() { return line_width.load(); }
  void set_trace_left(bool trace_left) { this->trace_left.store(trace_left); }
  ~CameraLineMonitor();
};

inline CameraLineMonitor::CameraLineMonitor()
    : diff(), mtx(), trace_left(true), line_width(0) {}

inline CameraLineMonitor::~CameraLineMonitor() {}

inline double CameraLineMonitor::get_differences() {
  mtx.lock();
  double ret = diff;
  mtx.unlock();
  return ret;
}

} // namespace informations

#endif // _CAMERA_LINE_MONITOR_HPP_
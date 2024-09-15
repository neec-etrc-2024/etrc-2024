#ifndef _CAMERA_LINE_MONITOR_HPP_
#define _CAMERA_LINE_MONITOR_HPP_

#include "FrameBuffer.hpp"
#include "gui/WindowManager.hpp"
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
  std::atomic_int blue_count;

public:
  CameraLineMonitor();
  double get_differences() override;
  void update(cv::Mat &img, FrameBuffer *buf = nullptr);
  int get_line_width() { return line_width.load(); }
  int get_blue_count() { return blue_count.load(); }
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
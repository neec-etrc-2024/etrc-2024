#ifndef _CAMERA_CONTROLLER_HPP_
#define _CAMERA_CONTROLLER_HPP_

#include <atomic>
#include <lccv.hpp>
#include <mutex>
#include <opencv2/opencv.hpp>

class CameraController {
private:
  lccv::PiCamera *cam;
  cv::Mat frame;
  std::mutex mtx;
  cv::VideoWriter videoWriter;
  std::atomic_bool is_ready;

public:
  CameraController() : cam(), is_ready(false) {};
  cv::Mat getFrame();
  void capture();
  bool ready() const { return is_ready.load(std::memory_order_acquire); };
};

#endif // _CAMERA_CONTROLLER_HPP_
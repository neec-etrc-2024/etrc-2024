#include "devices/CameraController.hpp"

#include "gui/WindowManager.hpp"

extern WindowManager wm;

void CameraController::capture() {
  cam = new lccv::PiCamera();
  cam->options->video_width = 1640;
  cam->options->video_height = 1232;
  cam->options->framerate = 30;
  bool res = cam->startVideo();

  if (!res) {
    std::cout << "Error: Camera start failed" << std::endl;
    return;
  }

  int fourcc = cv::VideoWriter::fourcc('m', 'p', '4', 'v');
  videoWriter.open(
      "output.mp4", fourcc, cam->options->framerate,
      cv::Size(cam->options->video_width, cam->options->video_height));

  is_ready.store(true, std::memory_order_release);
  while (true) {
    cv::Mat frame;
    if (!cam->getVideoFrame(frame, 100)) {
      std::cout << "Timeout" << std::endl;
      continue;
    }
    if (frame.empty()) {
      std::cout << "Error: Video frame is empty" << std::endl;
      continue;
    }
    mtx.lock();
    this->frame = frame;
    wm.update_window(0, frame);
    videoWriter << frame;
    mtx.unlock();
  }
}

cv::Mat CameraController::getFrame() {
  // printf("getFrame\n");
  cv::Mat ret;
  mtx.lock();
  frame.copyTo(ret);
  mtx.unlock();

  return ret;
}
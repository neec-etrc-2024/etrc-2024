#ifndef _CAMERA_CONTROLLER_HPP_
#define _CAMERA_CONTROLLER_HPP_

#include <mutex>

namespace devices {
class CameraController {
private:
  /* data */
public:
  CameraController(/* args */);
  ~CameraController();
};

inline CameraController::CameraController(/* args */) {}

inline CameraController::~CameraController() {}

} // namespace devices

#endif // _CAMERA_CONTROLLER_HPP_
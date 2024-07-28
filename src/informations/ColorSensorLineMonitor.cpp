#include "informations/ColorSensorLineMonitor.hpp"

namespace informations {
double ColorSensorLineMonitor::get_differences() const {
  pup_color_rgb_t rgb = this->color_sensor.get_rgb();
  int diff = rgb.b - 512;
  return diff / 512.0;
}
} // namespace informations
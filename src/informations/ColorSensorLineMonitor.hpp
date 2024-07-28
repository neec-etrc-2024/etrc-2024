#ifndef _COLOR_SENSOR_LINE_MONITOR_HPP_
#define _COLOR_SENSOR_LINE_MONITOR_HPP_

#include "ColorSensor.hpp"
#include <informations/ILineMonitor.hpp>

using namespace spikeapi;

namespace informations {
class ColorSensorLineMonitor : public ILineMonitor {
private:
  ColorSensor &color_sensor;

public:
  ColorSensorLineMonitor(ColorSensor &color_sensor);
  double get_differences() const override;
  ~ColorSensorLineMonitor();
};

inline ColorSensorLineMonitor::ColorSensorLineMonitor(ColorSensor &color_sensor)
    : color_sensor(color_sensor) {}

inline ColorSensorLineMonitor::~ColorSensorLineMonitor() {}

// inline double ColorSensorLineMonitor::get_differences() const { return 0; }

} // namespace informations

#endif // _COLOR_SENSOR_LINE_MONITOR_HPP_
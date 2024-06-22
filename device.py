from etrobo_python import Motor, ColorSensor


class Device:
    def set_device(self, left: Motor, right: Motor, color: ColorSensor):
        self.left_motor = left
        self.right_motor = right
        self.color_sensor = color

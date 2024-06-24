from etrobo_python import Motor, ColorSensor, Hub, GyroSensor, SonarSensor, TouchSensor


class Device:

    def __init__(
        self,
        hub: Hub,
        color_sensor: ColorSensor,
        gyro_sensor: GyroSensor,
        sonar_sensor: SonarSensor,
        touch_sensor: TouchSensor,
        left_motor: Motor,
        right_motor: Motor,
        arm_motor: Motor,
    ):
        self.hub = hub
        self.color_sensor = color_sensor
        self.gyro_sensor = gyro_sensor
        self.sonar_sensor = sonar_sensor
        self.touch_sensor = touch_sensor
        self.left_motor = left_motor
        self.right_motor = right_motor
        self.arm_motor = arm_motor

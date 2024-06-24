from etrobo_python import (
    ETRobo,
    Motor,
    ColorSensor,
    GyroSensor,
    SonarSensor,
    Hub,
    TouchSensor,
)
from device import Device

is_initialized = False
devices: Device = None


def main():
    etrobo = ETRobo(backend="simulator")
    etrobo.add_hub("hub")
    etrobo.add_device("color_sensor", ColorSensor, "2")
    etrobo.add_device("gyro_sensor", GyroSensor, "4")
    etrobo.add_device("sonar_sensor", SonarSensor, "3")
    etrobo.add_device("touch_sensor", TouchSensor, "1")
    etrobo.add_device("left_motor", Motor, "C")
    etrobo.add_device("right_motor", Motor, "B")
    etrobo.add_device("arm_motor", Motor, "A")
    etrobo.add_handler(handler)


def handler(
    hub: Hub,
    color_sensor: ColorSensor,
    gyro_sensor: GyroSensor,
    sonar_sensor: SonarSensor,
    touch_sensor: TouchSensor,
    left_motor: Motor,
    right_motor: Motor,
    arm_motor: Motor,
):
    global is_initialized
    if not is_initialized:
        is_initialized = True
        global devics
        devics = Device(
            hub,
            color_sensor,
            gyro_sensor,
            sonar_sensor,
            touch_sensor,
            left_motor,
            right_motor,
            arm_motor,
        )
    main_loop()


def main_loop():
    pass

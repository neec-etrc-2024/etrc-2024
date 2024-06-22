import abc
from etrobo_python import Motor, ColorSensor
from enum import Enum
from device import Device


class IRunner(metaclass=abc.ABCMeta):
    @abc.abstractmethod
    def run(self):
        raise NotImplementedError()


class Stop(IRunner):
    def __init__(self, device: Device) -> None:
        super().__init__()
        self.device = device

    def run(self):
        self.device.right_motor.set_power(0)
        self.device.left_motor.set_power(0)


class LineTrace(IRunner):
    class Mode(Enum):
        """LineTraceのモード 線を左右どちらに見るかを決める"""

        LEFT = 0
        RIGHT = 1

    def __init__(
        self,
        device: Device,
        mode: Mode = Mode.RIGHT,
        base_pwm=50,
        pid_p=0.2,
    ) -> None:
        super().__init__()
        self.device = device
        self.mode = mode
        self.base_pwm = base_pwm
        self.pid_p = pid_p

    def calc_pid(self, target: int, current: int):
        return self.pid_p * (current - target)

    def run(self):
        target = 17
        current = self.device.color_sensor.get_brightness()
        power_ratio = self.calc_pid(target, current)

        if self.mode == LineTrace.Mode.LEFT:  # 線を左に見る -> 大きい時左、小さい時右
            if power_ratio > 0:
                self.device.right_motor.set_power(self.base_pwm)
                self.device.left_motor.set_power(int(self.base_pwm / (1 + power_ratio)))
            else:
                self.device.right_motor.set_power(
                    int(self.base_pwm / (1 - power_ratio))
                )
                self.device.left_motor.set_power(self.base_pwm)
        else:  # 線を右に見る -> 大きい時右、小さい時左
            if power_ratio > 0:
                self.device.right_motor.set_power(
                    int(self.base_pwm / (1 + power_ratio))
                )
                self.device.left_motor.set_power(self.base_pwm)
            else:
                self.device.right_motor.set_power(self.base_pwm)
                self.device.left_motor.set_power(int(self.base_pwm / (1 - power_ratio)))

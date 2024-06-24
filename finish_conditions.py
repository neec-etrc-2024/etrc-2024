import abc
from device import Device


class IConditionsChecker(metaclass=abc.ABCMeta):
    @abc.abstractmethod
    def check(self) -> bool:
        raise NotImplementedError()


class ButtonPushedChecker(IConditionsChecker):
    def __init__(self, device: Device) -> None:
        super().__init__()
        self.hub = device.hub

    def check(self) -> bool:
        return self.hub.is_left_button_pressed or self.hub.is_right_button_pressed

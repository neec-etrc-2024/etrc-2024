import abc


class IConditionsChecker(metaclass=abc.ABCMeta):
    @abc.abstractmethod
    def check(self) -> bool:
        raise NotImplementedError()

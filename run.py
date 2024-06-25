# 走行
from abc import ABCMeta, abstractmethod

class Run(metaclass=ABCMeta):
    """走行インターフェース"""

    @abstractmethod
    def run() -> None:
        raise NotImplementedError
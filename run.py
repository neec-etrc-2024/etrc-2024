# 走行
from abc import ABCMeta, abstractmethod

class Run(metaclass=ABCMeta):
    @abstractmethod
    def run()->None:
        raise NotImplementedError
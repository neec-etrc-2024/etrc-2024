# 終了条件

from abc import ABCMeta, abstractmethod

class TerminationChecker(metaclass=ABCMeta):
    """終了条件を判定するインターフェイス"""
    
    @abstractmethod
    def check_met(self) -> bool:
        """終了条件を満たしているか確認する

        Returns:
            bool: 終了条件を満たしているか
        """
        raise NotImplementedError
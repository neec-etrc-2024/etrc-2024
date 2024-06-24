# 走行距離判定

from termination_checker import TerminationChecker

class TravelDistanceDecision(TerminationChecker):
        """走行距離判定"""
        
        def check_met(self) -> bool:
            return False
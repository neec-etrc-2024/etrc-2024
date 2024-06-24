# ラインまたぎ判定

from termination_checker import TerminationChecker

class DecisionSpanningLine(TerminationChecker):
    """ラインまたぎ判定"""
        
    def check_met(self) -> bool:
        return False
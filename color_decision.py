# 色判定

from termination_checker import TerminationChecker


class ColorDecision(TerminationChecker):
    """色判定"""

    def __init__(self) -> None:
        super().__init__()
        self.target_color = 0  # 検知する色

    def check_met(self) -> bool:
        return False

    def extract_color_from_frame(self) -> int:
        """フレームから色を取得する

        Returns:
            int: 色
        """
        return 0

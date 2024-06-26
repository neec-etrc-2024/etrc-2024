# モータ


class Motor:
    """モータ"""

    def __init__(self) -> None:
        self.speed = 0

    def set_speed(self, speed: int) -> None:
        """モータのスピードを設定する

        Args:
            speed (int): スピード
        """
        self.speed = speed

    def get_speed(self) -> int:
        """モータのスピードを取得する

        Returns:
            int: スピード
        """
        return self.speed

    def init(self) -> None:
        self.speed = 0

# モータ

class Motor:
    """モータ"""
    
    def __init__(self) -> None:
        self.speed = 0

    def set_speed(self, speed) -> None:
        """モータのスピードを設定する

        Args:
            speed (_type_): スピード
        """
        self.speed = speed

    def get_speed(self) -> int:
        """モータのスピードを取得する

        Returns:
            _type_: スピード
        """
        return self.speed
    
    def init(self) -> None:
        self.speed = 0
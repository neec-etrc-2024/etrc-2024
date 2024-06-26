import cv2
from abc import ABC, abstractmethod


class ITemplateMatcher(ABC):
    """
    テンプレートマッチングを行うためのインターフェース。

    Methods
    -------
    load_images(map_image_path: str = None, template_image_path: str = None):
        画像を読み込むメソッド。
    match_template():
        テンプレートマッチングを行うメソッド。
    show_result():
        マッチング結果を表示するメソッド。
    save_result(path: str):
        マッチング結果を保存するメソッド。
    print_match_info():
        マッチング結果の情報を表示するメソッド。
    """

    @abstractmethod
    def __init__(self) -> None:
        """
        テンプレートマッチングを行うクラスの初期化メソッド。

        Parameters:
        ----------
        map_image_path (str): マッチングを行う対象の画像のパス。
        template_image_path (str): マッチングに使用するテンプレート画像のパス。
        threshold (float, optional): マッチングの閾値。この値以上のマッチング結果のみを有効とする。デフォルトは0.8。
        """
        pass

    @abstractmethod
    def load_images(self, map_image_path: str = None, template_image_path: str = None):
        """
        画像を読み込むメソッド。引数が与えられた場合はそのパスから画像を読み込み、
        そうでない場合は初期化時に指定されたパスから画像を読み込む。

        Parameters:
        ----------
        map_image_path (str, optional): マッチングを行う対象の画像のパス。デフォルトはNone。
        template_image_path (str, optional): マッチングに使用するテンプレート画像のパス。デフォルトはNone。
        """
        pass

    @abstractmethod
    def match_template(self):
        """テンプレートマッチングを行うメソッド

        Raises
        ------
        ValueError
            画像が読み込まれていない場合に発生。
        """
        pass

    @abstractmethod
    def show_result(self):
        """マッチング結果を表示するメソッド

        Raises
        ------
        ValueError
            画像が読み込まれていない場合に発生。
        """
        pass

    @abstractmethod
    def save_result(self, path: str):
        """マッチング結果を保存するメソッド

        Parameters
        ----------
        path : str
            保存先のパス。

        Raises
        ------
        ValueError
            画像が読み込まれていない場合に発生。
        """
        pass

    @abstractmethod
    def print_match_info(self):
        """マッチング結果の情報を表示するメソッド

        Raises
        ------
        ValueError
            マッチング結果がない場合に発生。
        """
        pass

    @abstractmethod
    def get_max_val(self) -> float | None:
        """マッチング結果の信頼度を返すメソッド"""
        pass


class TemplateMatcher(ITemplateMatcher):
    """テンプレートマッチングを行うクラス

    example
    -----
    ```python
    template_matcher = TemplateMatcher("map_image_path", "template_image_path")
    template_matcher.load_images()
    template_matcher.match_template()
    print(template_matcher.get_max_val())
    template_matcher.show_result()
    template_matcher.print_match_info()
    template_matcher.save_result("result.png")
    ```
    """

    def __init__(
        self, map_image_path: str, template_image_path: str, threshold: float = 0.8
    ):
        self.threshold = threshold
        self.map_image_path = map_image_path
        self.template_image_path = template_image_path
        self.map_image = None
        self.template_image = None
        self.match_result = None
        self.top_left = None
        self.bottom_right = None
        self.max_val = None
        self.max_loc = None

    def load_images(self, map_image_path: str = None, template_image_path: str = None):
        if map_image_path is not None:
            self.map_image_path = map_image_path
        if template_image_path is not None:
            self.template_image_path = template_image_path

        self.map_image = cv2.imread(self.map_image_path)
        self.template_image = cv2.imread(self.template_image_path)

    def match_template(self):
        if self.map_image is None or self.template_image is None:
            raise ValueError("画像が読み込まれていません。")
        w, h, _ = self.template_image.shape
        matching_results = cv2.matchTemplate(
            self.map_image, self.template_image, cv2.TM_CCOEFF_NORMED
        )
        _, self.max_val, _, self.max_loc = cv2.minMaxLoc(matching_results)
        self.top_left = self.max_loc
        self.bottom_right = (self.top_left[0] + w, self.top_left[1] + h)

    def __draw_rectangle(self):
        """マッチング結果に矩形を描画するメソッド"""
        if self.top_left is None or self.bottom_right is None:
            raise ValueError("マッチング結果がありません。")
        cv2.rectangle(
            self.map_image, self.top_left, self.bottom_right, (255, 255, 0), 2
        )

    def show_result(self):
        self.__draw_rectangle()
        if self.map_image is None:
            raise ValueError("画像が読み込まれていません。")
        cv2.imshow("Match Result", self.map_image)
        cv2.waitKey(0)

    def save_result(self, path: str):
        if self.map_image is None:
            raise ValueError("画像が読み込まれていません。")
        cv2.imwrite(path, self.map_image)

    def print_match_info(self):
        if self.max_val is None or self.max_loc is None:
            raise ValueError("マッチング結果がありません。")
        print("信頼度: ", self.max_val * 100, "%")
        print("位置座標: ", self.max_loc)

    def get_max_val(self) -> float | None:
        return self.max_val


if __name__ == "__main__":
    template_matcher = TemplateMatcher("./img/map.jpg", "./img/course_in8_60_mark.jpg")
    template_matcher.load_images()
    template_matcher.match_template()
    template_matcher.show_result()
    print(template_matcher.get_max_val())
    template_matcher.print_match_info()
    template_matcher.save_result("result.png")

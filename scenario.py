from finish_conditions import IConditionsChecker
from runner import IRunner


class Scene:
    def __init__(self, cond: IConditionsChecker, runner: IRunner) -> None:
        self.cond = cond
        self.runner = runner

    def run(self) -> bool:
        self.runner.run()
        return self.cond.check()


class Scenario:

    def __init__(self) -> None:
        self.scenes: list[Scene] = []
        self.scene_index = 0

    def run(self) -> bool:
        if self.scene_index >= len(self.scenes):
            return True
        result = self.scenes[self.scene_index].run()
        if result:
            self._change_scene()
        return False

    def _change_scene(self):
        self.scene_index += 1

    def append(self, scene: Scene):
        self.scenes.append(scene)


class ScenarioTracer:
    def run(self) -> bool:
        pass

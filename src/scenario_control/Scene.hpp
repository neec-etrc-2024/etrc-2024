#ifndef _SCENE_HPP_
#define _SCENE_HPP_

#include "behaviors/IBehavior.hpp"
#include "conditions/ICondition.hpp"

using namespace behaviors;
using namespace conditions;

namespace scenario_control {
class Scene {
private:
  IBehavior &behavior;
  ICondition &condition;

public:
  Scene(IBehavior &behavior, ICondition &condition);
  bool run();
  ~Scene();
};

inline Scene::Scene(IBehavior &behavior, ICondition &condition)
    : behavior(behavior), condition(condition) {}

inline Scene::~Scene() {}

} // namespace scenario_control

#endif // _SCENE_HPP_
#pragma once

#include "termination_checker.hpp"

// ラインまたぎ判定クラス
class DecisionSpanningLine : public TerminationChecker
{
public:
    bool check_met() override;
};

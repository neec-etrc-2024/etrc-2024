#pragma once

#include "termination_checker.hpp"

// 色判定クラス
class ColorDecision : public TerminationChecker
{
public:
    ColorDecision();
    bool check_met() override;
    int extract_color_from_frame();

private:
    int target_color = 0; // 検知する色
};
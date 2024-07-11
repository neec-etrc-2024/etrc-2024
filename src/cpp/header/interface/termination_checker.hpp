#pragma once

// 終了条件を判定するインターフェース
class TerminationChecker
{
public:
    virtual bool check_met() = 0;            // 純粋仮想関数
    virtual ~TerminationChecker() = default; // 仮想デストラクタ
};

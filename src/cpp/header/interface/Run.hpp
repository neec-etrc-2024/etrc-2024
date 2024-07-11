#pragma once

// 走行インターフェース
class Run
{
public:
    virtual void run() = 0;   // 純粋仮想関数
    virtual ~Run() = default; // 仮想デストラクタ
};
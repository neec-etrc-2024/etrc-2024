#pragma once

class camera
{
public:
    virtual void get_frame() = 0; // 純粋仮想関数
    virtual ~camera() = default;  // 仮想デストラクタ
};
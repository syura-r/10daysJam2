#pragma once
#include "Object.h"
class BossHair :
    public Object
{
public:
    BossHair(const Vector3& pos,const int arg_deadTime);
    void Update()override;
    void Draw()override;
    void DrawReady()override;

private:
    int deadCounter;
    int deadTime;
    const float rotVel = 10;
};


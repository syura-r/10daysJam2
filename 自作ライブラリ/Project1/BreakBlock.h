#pragma once
#include "Object.h"
class BreakBlock :
    public Object
{
public:
    BreakBlock(const Vector3& position, const Vector3& scale = 1.0f);
    void OnCollision(const CollisionInfo& info)override;
};


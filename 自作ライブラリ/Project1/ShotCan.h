#pragma once
#include "Object.h"
class ShotCan :
    public Object
{
public:
    ShotCan(const Vector3& pos,const Vector3& shotVec);
    void Update()override;
    void Draw()override;
    void DrawReady()override;
    void OnCollision(const CollisionInfo& info)override;
private:
    float fallV = 0.02f;
    int aliveCount;
    float rotV ;

};


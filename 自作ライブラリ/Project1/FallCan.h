#pragma once
#include "Object.h"
class FBXModel;
class FallCan :
    public Object
{
public:
    FallCan(const Vector3& pos);
    void Update()override;
    void Draw()override;
    void DrawReady()override;
private:
    float fallV = 0.02f;
    int aliveCount;
    float rotV = 0.1f;
    FBXModel* myModel;
};


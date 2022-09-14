#pragma once
#include "Object.h"
class FBXModel;
class EndChara :
    public Object
{
public:
    EndChara();
    void Update()override;
    void Draw()override;
    void DrawReady()override;
private:
    FBXModel* myModel;
    bool actionStart = false;
    bool a = false;
};


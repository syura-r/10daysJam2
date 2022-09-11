#pragma once
#include "Object.h"
class Apartment :
    public Object
{
public:
    Apartment(const Vector3& arg_pos);
    void Draw()override;
    void DrawReady()override;
private:

};


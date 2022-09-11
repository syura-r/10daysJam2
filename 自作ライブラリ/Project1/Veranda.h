#pragma once
#include "Object.h"
class Veranda :
    public Object
{
public:
	Veranda(const Vector3& position, const Vector3& scale, const Vector3& rotation);
	void Draw()override;
	void DrawReady()override;

};


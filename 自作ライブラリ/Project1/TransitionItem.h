#pragma once
#include "Object3D.h"

class TransitionItem
{
public:
	TransitionItem();
	~TransitionItem();

	void Initialize(
		const float& arg_firstPositionX,
		const float& arg_firstPositionY,
		const float& arg_rotationZ);
	void Update(const Vector3& arg_parentPosition);
	void Draw();

	Vector3 GetAddPosition();

private:
	Object3D* obj = nullptr;
	Vector3 position = { 0,0,0 };
	Vector3 scale = { 1.5f,1.5f,1.5f };
	Vector3 rotation = { 0,0,0 };
	XMFLOAT4 color = { 1,1,1,1 };

	Vector3 randAddPosition = { 0,0,0 };
};


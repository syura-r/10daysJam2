#pragma once
#include "Object3D.h"
class CanInTitle
{
public:
	CanInTitle(const float arg_positionX_first,
		const float arg_positionY_first, const float arg_positionY_end,
		const float arg_rotationZ_first, const float arg_rotationZ_end);
	~CanInTitle();

	void Initialize();
	void Update();
	void Draw();

	bool GetIsMoveEnd() { return isMoveEnd; }

private:
	Object3D* object = nullptr;
	Vector3 position = { 0,0,0 };
	Vector3 scale = { 2.5f,2.7f,2.5f };
	Vector3 rotation = { 0,0,90 };
	XMFLOAT4 color = { 1,1,1,1 };


	void FrameIn();
	bool isMoveEnd = false;

	float positionX_first = 0.0f;
	float positionY_first = 0.0f;
	float positionY_end = 0.0f;

	float rotationZ_first = 0.0f;
	float rotationZ_end = 0.0f;

	//イージング用のタイマー
	int easingCount_position = 0;
	const int easingCountLimit_position = 60;
};


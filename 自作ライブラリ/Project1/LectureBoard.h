#pragma once
#include "Sprite3D.h"

class LectureBoard
{
public:
	LectureBoard();
	~LectureBoard();

	void Initialize();
	void Update();
	void Draw();

private:
	Sprite3D* sprite = nullptr;
	std::string texName;
	Vector3 position = {};

	float alpha = 0.0f;
	int easingCount = 0;

	bool inputClose = false;
};


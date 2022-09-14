#pragma once
#include "BaseEnemy.h"
class Crow :
    public BaseEnemy
{
public:
	Crow();
	~Crow();
	void Initialize()override;
	void Update()override;
private:
	int moveCounter;
	float targetPosY;
	const float attackVel = 0.15f;
	const float mmoveVel = 1.0f;

	void CryVoiceLoop();
	bool isSeLoop = false;
	int seIntervalCount = 0;
};


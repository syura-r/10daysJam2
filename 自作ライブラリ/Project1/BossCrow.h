#pragma once
#include "BaseEnemy.h"
class Boss;
class BossCrow :
    public BaseEnemy
{
public:
	BossCrow(Boss* bossPtr);
	~BossCrow();
	void Initialize()override;
	void Update()override;
private:

	void CryVoiceLoop();
	bool isSeLoop = false;
	int seIntervalCount = 0;

	Boss* boss = nullptr;

};

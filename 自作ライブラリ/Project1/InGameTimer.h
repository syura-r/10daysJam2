#pragma once
#include "Timer.h"
#include "Sprite.h"
#include "NumberSprite.h"

class InGameTimer
{
public:
	InGameTimer();
	~InGameTimer();

	void Initialize();
	void Update();
	void Draw();

	bool GetLimit() { return timer->IsTime(); }
	int GetRealTime() { return timer->GetRealTime(); }

private:
	float seconds = 0.0f;
	Timer* timer = nullptr;

	//ÉAÉCÉRÉì
	Sprite* stopwatch_sprite = nullptr;
	//ïb
	NumberSprite* seconds_sprite = nullptr;
};


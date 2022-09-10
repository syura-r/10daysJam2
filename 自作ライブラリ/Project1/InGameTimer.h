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

	//�A�C�R��
	Sprite* stopwatch_sprite = nullptr;
	//�b
	NumberSprite* seconds_sprite = nullptr;
};


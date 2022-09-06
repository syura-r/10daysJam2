#pragma once
#include "Timer.h"
#include "NumberSprite.h"
#include "Vector.h"
#include<unordered_map>

class TimeLimit
{
public:
	TimeLimit();
	~TimeLimit();

	void Initialize();
	void Update();
	void Draw();

	bool GetLimit() { return timer->IsTime(); }
	int GetNowTime() { return timer->GetTime(); }

private:
	float seconds = 0.0f;
	Timer* timer = nullptr;

	//ƒAƒCƒRƒ“
	Sprite* stopwatch_sprite = nullptr;
	//•b
	NumberSprite* seconds_sprite = nullptr;
	//Œ…”
	const int timerDigits = 3;
};


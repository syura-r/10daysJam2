#pragma once
#include "Vector.h"
#include "Sprite.h"
#include "NumberSprite.h"

class StockCansBar
{
public:
	StockCansBar();
	~StockCansBar();

	void Initialize(const float arg_firstCansCount);
	void Update(const float arg_nowCansCount);
	void Draw();

private:
	Sprite* bar_outside = nullptr;
	Sprite* bar_inside = nullptr;
	Sprite* bar_icon = nullptr;

	NumberSprite* cansCountSprite = nullptr;

	float barinside_sizeX = 0.0f;
	const Vector2 barSize_max = { 640.0f, 32.0f };

	//ŠÊ‚Ì‰Šú”‚ÆŒ»İ”
	float firstCansCount = 0.0f;
	float nowCansCount = 0.0f;

};


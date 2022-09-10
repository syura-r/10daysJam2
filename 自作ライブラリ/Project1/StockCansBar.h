#pragma once
#include "Vector.h"
#include "Sprite.h"
#include "NumberSprite.h"
#include "Timer.h"

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
	Sprite* bar_inside_sub = nullptr;//どれくらい減ったか
	Sprite* bar_icon = nullptr;

	NumberSprite* cansCountSprite = nullptr;

	//長さ
	float barSize_x_inside = 0.0f;
	float barSize_x_inside_sub = 0.0f;
	const Vector2 barSize_max = { 640.0f, 32.0f };

	//缶の初期数と現在数
	float firstCansCount = 0.0f;
	float nowCansCount = 0.0f;
	float subCansCount = 0.0f;//差分

	//差分表示を縮める
	struct SubSizeChange
	{
		int subCount = 0;
		int timer = 0;
	};
	std::vector<SubSizeChange> array_subSizeChange;
	const int limitTime = 20;//何フレーム後に縮み始めるか

};


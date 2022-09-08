#pragma once
#include "Sprite.h"
#include "NumberSprite.h"
#include "Timer.h"
#include "Vector.h"

class Result
{
public:
	Result();
	~Result();

	void Initialize();
	void Update();
	void Draw();

	bool GetActivePause() { return activeFlag; }
	void IsActive(const float arg_canCount, const float arg_jumpCount);

	bool GetIsToNextScene() { return isToNextScene; }

private:
	//起動時に各数値を受け取る
	void SetScores(const float arg_canCount, const float arg_jumpCount);
	//
	bool activeFlag = false;

	//ゲーム画面を暗く
	Sprite* sp_back = nullptr;

	//缶の数とジャンプの回数
	Sprite* canCountSp = nullptr;
	Sprite* jumpCountSp = nullptr;
	NumberSprite* canCountNumSp = nullptr;
	NumberSprite* jumpCountNumSp = nullptr;
	float canCount = 0.0f;
	float jumpCount = 0.0f;
	Vector2 position_can = {};
	Vector2 position_jump = {};

	//横から出てくる
	void SlideIn();
	const float positionX_slideIn_start = 1920.0f + 128.0f;
	const float positionX_slideIn_end = 1920.0f - 320.0f;


	//表示の段階
	int step_display = 0;
	//総段階数
	const int stepLimit_display = 2;
	//インターバルを数える
	Timer* timer_display = nullptr;
	const int timerLimit_display = 20;


	//シーン切り替え可能か
	bool isSceneChangeStandby = false;
	//シーン切り替えする
	bool isToNextScene = false;


	//決定ボタン表示
	Sprite* button = nullptr;
	float alpha_button = 0.0f;

};


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

	//残った缶の数とジャンプの回数などを表示するモノ
	struct CountDisplayer
	{
		CountDisplayer();
		~CountDisplayer();

		void Initialize(const float arg_position_y);
		void Update();
		void Draw();

		Sprite* sprite = nullptr;
		NumberSprite* numberSprite = nullptr;
		//表示する数値
		float count = 0.0f;
		//ベースのスプライトの座標
		Vector2 position_sp = {};

		//
		void SlideIn(const float limitTime, const float nowTime);
		const float position_slideIn_start_x = 1920.0f + 256.0f;
		const float position_slideIn_end_x = 1920.0f - 256.0f;
	};

	CountDisplayer* countDisplyer_can = nullptr;
	CountDisplayer* countDisplyer_jump = nullptr;


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


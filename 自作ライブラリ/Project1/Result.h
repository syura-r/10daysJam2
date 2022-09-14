#pragma once
#include "Vector.h"
#include "NumberSprite.h"
#include "Sprite.h"

class Result
{
public:
	static Result* GetInstance();
	void Finalize();

	void Initialize();
	void Update();
	void Draw();

	bool GetActive() { return activeFlag; }

	void SetCanCount(const float arg_canCount);
	void SetTimeCount(const float arg_timeCount);
	void IsActive(const bool arg_isClear);

	bool GetIsCloseResult() { return isCloseResult; }

private:
	//
	Result();
	~Result();
	static Result* instance;

	bool activeFlag = false;

	//ゲーム画面を暗く
	Sprite* sp_back = nullptr;

	//成績表示用
	struct CountDisplayer
	{
		CountDisplayer();
		~CountDisplayer();

		void Initialize(const float arg_position_y);
		void Update();
		void Draw(const std::string& arg_baseTexName);

		Sprite* sprite = nullptr;
		NumberSprite* numberSprite = nullptr;
		//表示する数値
		float count = 0.0f;
		//ベースのスプライトの座標
		Vector2 position_sp = {};

		//
		void SlideIn(const float limitTime, const float nowTime);
		const float position_slideIn_start_x = 1920.0f + 300.0f;
		const float position_slideIn_end_x = 1200.0f;
	};

	//残量
	CountDisplayer* countDisplyer_can = nullptr;
	//タイム
	CountDisplayer* countDisplyer_time = nullptr;

	//
	bool isClear = false;
	Sprite* sprite_clear = nullptr;
	float rotation_clear = 0.0f;
	Vector2 scale_clear = { 1,1 };
	float alpha_clear = 0.0f;

	//表示の段階
	int step_display = 0;
	//総段階数
	const int stepLimit_display = 3;
	//インターバルを数える
	int timer = 0;
	const int timerLimit = 20;


	//リザルトを閉じれる状態か
	bool isCloseResult_standby = false;
	//リザルトを閉じた
	bool isCloseResult = false;


	//決定ボタン表示
	Sprite* button = nullptr;
	float alpha_button = 0.0f;

};


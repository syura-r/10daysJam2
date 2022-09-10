#pragma once
#include "Sprite.h"
#include "Vector.h"

class Pause
{
public:
	Pause();
	~Pause();

	void Initialize();
	void Update();
	void Draw();

	bool GetActivePause() { return activeFlag; }

	bool GetToGame() { return flag_toGame; }
	bool GetRestart() { return flag_restart; }
	bool GetToTitle() { return flag_toTitle; }

private:
	//選択肢を選ぶ処理
	void Select();
	int inputInterval_UD = 0;
	const int inputInterval = 15;
	//決定ボタンを押した後の処理
	void Decision();

	//描画や更新処理を行うか
	bool activeFlag = false;
	//
	static bool fadeFlag;


	enum SelectState
	{
		ToGame = 0,
		Restart = 1,
		ToTitle = 2,
		BGM = 3,
		SE = 4,
	};
	SelectState selectState;
	//選択肢の個数
	const int selectMax = 5;

	//ゲーム画面を暗く
	Sprite* sp_back = nullptr;
	Vector2 pos_back = {};

	//選択を見やすく
	Sprite* sp_base = nullptr;
	Vector2 pos_base = {};
	float alpha_base = 1.0f;
	bool isUP_alphaChange = false;

	//選択肢1つに必要な変数
	struct SelectSprite
	{
		SelectSprite();
		~SelectSprite();
		void Initialize(const std::string& texName, const float posY);
		void Update();
		void Draw();

		Sprite* sprite = nullptr;
		std::string texName = "";
		Vector2 pos = {};
	};


	//ゲームにもどる
	SelectSprite* toGame = nullptr;
	bool flag_toGame = false;

	//やり直す
	SelectSprite* restart = nullptr;
	bool flag_restart = false;

	//タイトルにもどる
	SelectSprite* toTitle = nullptr;
	bool flag_toTitle = false;

	//音量設定
	void VolumeChange();
	int inputInterval_LR = 0;

	//音量設定BGM
	SelectSprite* bgm = nullptr;
	Sprite* bar_bgm;
	Vector2 barPositionLeft_bgm;
	const Vector2 bar_scale = { 256.0f,5.0f };
	Sprite* circle_bgm;
	Vector2 circlePosition_bgm;

	//音量設定SE
	SelectSprite* se = nullptr;
	Sprite* bar_se;
	Vector2 barPositionLeft_se;
	Sprite* circle_se;
	Vector2 circlePosition_se;
};

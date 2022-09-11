#pragma once
#include "Sprite.h"
#include "Vector.h"

class GameEndSelect
{
public:
	GameEndSelect();
	~GameEndSelect();

	void Initialize();
	void Update();
	void Draw();

	void IsActive();
	bool GetIsActive() { return activeFlag; }
	bool GetRestart() { return flag_restart; }
	bool GetToTitle() { return flag_toTitle; }

private:
	//選択肢を選ぶ処理
	void Select();
	//決定ボタンを押した後の処理
	void Decision();

	//描画や更新処理を行うか
	bool activeFlag = false;

	enum SelectState
	{
		Restart = 0,
		ToTitle = 1,
	};
	SelectState selectState;
	//選択肢の個数
	const int selectMax = 2;

	//ゲーム画面を暗く
	Sprite* sp_back = nullptr;
	Vector2 pos_back = {};

	//選択を見やすく
	Sprite* sp_base = nullptr;
	Vector2 pos_base = {};
	float alpha_base = 1.0f;
	bool isUP_alphaChange = false;

	//選択肢1つに必要な変数
	struct SelectSprite_inGameEndSelect
	{
		SelectSprite_inGameEndSelect();
		~SelectSprite_inGameEndSelect();
		void Initialize(const std::string& texName, const float posY);
		void Update();
		void Draw();

		Sprite* sprite = nullptr;
		std::string texName = "";
		Vector2 pos = {};
	};


	//やり直す
	SelectSprite_inGameEndSelect* restart = nullptr;
	bool flag_restart = false;

	//タイトルにもどる
	SelectSprite_inGameEndSelect* toTitle = nullptr;
	bool flag_toTitle = false;
};


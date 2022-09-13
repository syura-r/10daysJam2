#pragma once
#include "Sprite.h"
#include "NumberSprite.h"
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
	void IsActive(const bool arg_isClear, const float arg_canCount, const float arg_time);

	bool GetIsCloseResult() { return isCloseResult; }

private:
	//
	bool activeFlag = false;

	//�Q�[����ʂ��Â�
	Sprite* sp_back = nullptr;

	//���ѕ\���p
	struct CountDisplayer
	{
		CountDisplayer();
		~CountDisplayer();

		void Initialize(const float arg_position_y);
		void Update();
		void Draw(const std::string& arg_baseTexName);

		Sprite* sprite = nullptr;
		NumberSprite* numberSprite = nullptr;
		//�\�����鐔�l
		float count = 0.0f;
		//�x�[�X�̃X�v���C�g�̍��W
		Vector2 position_sp = {};

		//
		void SlideIn(const float limitTime, const float nowTime);
		const float position_slideIn_start_x = 1920.0f + 300.0f;
		const float position_slideIn_end_x = 1200.0f;
	};

	//�c��
	CountDisplayer* countDisplyer_can = nullptr;
	//�^�C��
	CountDisplayer* countDisplyer_time = nullptr;

	//
	bool isClear = false;
	Sprite* sprite_clear = nullptr;
	float rotation_clear = 0.0f;
	Vector2 scale_clear = { 1,1 };
	float alpha_clear = 0.0f;

	//�\���̒i�K
	int step_display = 0;
	//���i�K��
	const int stepLimit_display = 3;
	//�C���^�[�o���𐔂���
	int timer = 0;
	const int timerLimit = 20;


	//���U���g�������Ԃ�
	bool isCloseResult_standby = false;
	//���U���g�����
	bool isCloseResult = false;


	//����{�^���\��
	Sprite* button = nullptr;
	float alpha_button = 0.0f;

};


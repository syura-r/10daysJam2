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
	//�N�����Ɋe���l���󂯎��
	void SetScores(const float arg_canCount, const float arg_jumpCount);
	//
	bool activeFlag = false;

	//�Q�[����ʂ��Â�
	Sprite* sp_back = nullptr;

	//�c�����ʂ̐��ƃW�����v�̉񐔂Ȃǂ�\�����郂�m
	struct CountDisplayer
	{
		CountDisplayer();
		~CountDisplayer();

		void Initialize(const float arg_position_y);
		void Update();
		void Draw();

		Sprite* sprite = nullptr;
		NumberSprite* numberSprite = nullptr;
		//�\�����鐔�l
		float count = 0.0f;
		//�x�[�X�̃X�v���C�g�̍��W
		Vector2 position_sp = {};

		//
		void SlideIn(const float limitTime, const float nowTime);
		const float position_slideIn_start_x = 1920.0f + 256.0f;
		const float position_slideIn_end_x = 1920.0f - 256.0f;
	};

	CountDisplayer* countDisplyer_can = nullptr;
	CountDisplayer* countDisplyer_jump = nullptr;


	//�\���̒i�K
	int step_display = 0;
	//���i�K��
	const int stepLimit_display = 2;
	//�C���^�[�o���𐔂���
	Timer* timer_display = nullptr;
	const int timerLimit_display = 20;


	//�V�[���؂�ւ��\��
	bool isSceneChangeStandby = false;
	//�V�[���؂�ւ�����
	bool isToNextScene = false;


	//����{�^���\��
	Sprite* button = nullptr;
	float alpha_button = 0.0f;

};


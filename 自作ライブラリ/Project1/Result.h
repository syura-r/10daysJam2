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

	//�ʂ̐��ƃW�����v�̉�
	Sprite* canCountSp = nullptr;
	Sprite* jumpCountSp = nullptr;
	NumberSprite* canCountNumSp = nullptr;
	NumberSprite* jumpCountNumSp = nullptr;
	float canCount = 0.0f;
	float jumpCount = 0.0f;
	Vector2 position_can = {};
	Vector2 position_jump = {};

	//������o�Ă���
	void SlideIn();
	const float positionX_slideIn_start = 1920.0f + 128.0f;
	const float positionX_slideIn_end = 1920.0f - 320.0f;


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


#pragma once
#include "Vector.h"
#include "Sprite.h"
#include "NumberSprite.h"
#include "Timer.h"

class HitPointBar
{
public:
	HitPointBar();
	~HitPointBar();

	void Initialize(const float arg_firstCount);
	void Update(const float arg_nowCount);
	void Draw_player();
	void Draw_boss();

private:
	Sprite* bar_outside = nullptr;
	Sprite* bar_inside = nullptr;
	Sprite* bar_inside_sub = nullptr;//�ǂꂭ�炢��������
	Sprite* bar_icon = nullptr;

	NumberSprite* countSprite = nullptr;

	//����
	float barSize_x_inside = 0.0f;
	float barSize_x_inside_sub = 0.0f;
	const Vector2 barSize_max = { 640.0f, 64.0f };

	//�ʂ̏������ƌ��ݐ�
	float firstCount = 0.0f;
	float nowCount = 0.0f;
	float subCount = 0.0f;//����

	//�����\�����k�߂�
	struct SubSizeChange
	{
		int subCount = 0;
		int timer = 0;
	};
	std::vector<SubSizeChange> array_subSizeChange;
	const int limitTime = 20;//���t���[����ɏk�ݎn�߂邩

};


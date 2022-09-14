#include "HitPointBar.h"

HitPointBar::HitPointBar()
{
	bar_outside = new Sprite();
	bar_inside = new Sprite();
	bar_inside_sub = new Sprite();
	bar_icon = new Sprite();

	countSprite = new NumberSprite(nowCount);
}

HitPointBar::~HitPointBar()
{
	delete bar_outside;
	delete bar_inside;
	delete bar_inside_sub;
	delete bar_icon;
	delete countSprite;
}

void HitPointBar::Initialize(const float arg_firstCount)
{
	firstCount = arg_firstCount;
	nowCount = firstCount;
	subCount = 0;
	barSize_x_inside = barSize_max.x;

	array_subSizeChange.clear();
}

void HitPointBar::Update(const float arg_nowCount)
{
	const int sub = nowCount - arg_nowCount;
	if (sub && nowCount > 0.0f)
	{
		array_subSizeChange.push_back({ sub,0 });
	}
	nowCount = arg_nowCount;
	if (nowCount < 0.0f)
	{
		nowCount = 0.0f;
	}


	//�������k�߂�܂ł̃^�C�}�[
	for (int i = 0; i < array_subSizeChange.size(); i++)
	{
		//���ԂɂȂ�����
		if (array_subSizeChange[i].timer >= limitTime)
		{
			//�k�߂�
			const float speed = 1.0f;
			array_subSizeChange[i].subCount -= speed;

			//0�ȉ��ɂȂ��������
			if (array_subSizeChange[i].subCount <= 0.0f)
			{
				array_subSizeChange.erase(array_subSizeChange.begin() + i);
			}
		}
		else
		{
			array_subSizeChange[i].timer++;
		}
	}

	//�����̍��v
	subCount = 0.0f;
	for (int i = 0; i < array_subSizeChange.size(); i++)
	{
		subCount += array_subSizeChange[i].subCount;
	}

	//�����ύX
	if (firstCount <= nowCount)
	{
		barSize_x_inside = barSize_max.x;
		//
		barSize_x_inside_sub = barSize_max.x;
	}
	else
	{
		barSize_x_inside = (barSize_max.x / firstCount) * nowCount;
		//
		barSize_x_inside_sub = (barSize_max.x / firstCount) * subCount
			+ barSize_x_inside;
	}
}

void HitPointBar::Draw_player()
{
	const Vector2 position = { 100, 150 };
	const Vector4 color_inside = { 0.1f,0.2f,0.9f,1 };//blue
	const Vector4 color_inside_sub = { 0.9f,0.1f,0.2f,1 };//red

	//�A�C�R��
	countSprite->Draw(std::to_string((int)nowCount).size(), "number", position);
	bar_icon->DrawSprite("can", position, -10.0f, { 0.45f, 0.45f });

	//�t���[��
	bar_outside->DrawSprite("frame", position, 0.0f, { 1,1 }, { 1,1,1,1 }, { 0.0f,0.5f });

	//�Q�[�W
	bar_inside->DrawSprite("white1x1", position, 0.0f, { barSize_x_inside, barSize_max.y }, color_inside, { 0.0f,0.5f });
	bar_inside_sub->DrawSprite("white1x1", position, 0.0f, { barSize_x_inside_sub, barSize_max.y }, color_inside_sub, { 0.0f,0.5f });
}

void HitPointBar::Draw_boss()
{
	const Vector2 position = { 1820, 150 };
	const Vector4 color_inside = { 0.5f,0.0f,0.0f,1 };//deepRed
	const Vector4 color_inside_sub = { 0.9f,0.1f,0.2f,1 };//red

	//�t���[��
	bar_outside->DrawSprite("frame", position, 0.0f, { 1,1 }, { 1,1,1,1 }, { 1.0f,0.5f });

	//�Q�[�W
	bar_inside->DrawSprite("white1x1", position, 0.0f, { barSize_x_inside, barSize_max.y }, color_inside, { 1.0f,0.5f });
	bar_inside_sub->DrawSprite("white1x1", position, 0.0f, { barSize_x_inside_sub, barSize_max.y }, color_inside_sub, { 1.0f,0.5f });
}

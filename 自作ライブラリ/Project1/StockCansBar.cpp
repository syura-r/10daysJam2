#include "StockCansBar.h"

StockCansBar::StockCansBar()
{
	bar_outside = new Sprite();
	bar_inside = new Sprite();
	bar_inside_sub = new Sprite();
	bar_icon = new Sprite();

	cansCountSprite = new NumberSprite(nowCansCount);
}

StockCansBar::~StockCansBar()
{
	delete bar_outside;
	delete bar_inside;
	delete bar_inside_sub;
	delete bar_icon;
	delete cansCountSprite;
}

void StockCansBar::Initialize(const float arg_firstCansCount)
{
	firstCansCount = arg_firstCansCount;
	nowCansCount = firstCansCount;
	subCansCount = 0;
	barSize_x_inside = barSize_max.x;

	array_subSizeChange.clear();
}

void StockCansBar::Update(const float arg_nowCansCount)
{
	const int sub = nowCansCount - arg_nowCansCount;
	if (sub)
	{
		array_subSizeChange.push_back({ sub,0 });
	}
	nowCansCount = arg_nowCansCount;
	if (nowCansCount < 0.0f)
	{
		nowCansCount = 0.0f;
	}


	//差分を縮めるまでのタイマー
	for (int i = 0; i < array_subSizeChange.size(); i++)
	{
		//時間になったら
		if (array_subSizeChange[i].timer >= limitTime)
		{
			//縮める
			const float speed = 1.0f;
			array_subSizeChange[i].subCount -= speed;

			//0以下になったら消す
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

	//差分の合計
	subCansCount = 0.0f;
	for (int i = 0; i < array_subSizeChange.size(); i++)
	{
		subCansCount += array_subSizeChange[i].subCount;
	}

	//長さ変更
	if (firstCansCount <= nowCansCount)
	{
		barSize_x_inside = barSize_max.x;
		//
		barSize_x_inside_sub = barSize_max.x;
	}
	else
	{
		barSize_x_inside = (barSize_max.x / firstCansCount) * nowCansCount;
		//
		barSize_x_inside_sub = (barSize_max.x / firstCansCount) * subCansCount
			+ barSize_x_inside;
	}
}

void StockCansBar::Draw()
{
	const Vector2 position = {100, 150};

	cansCountSprite->Draw(std::to_string((int)nowCansCount).size(), "number", position);

	bar_icon->DrawSprite("can", position, -10.0f, { 0.45f, 0.45f });

	bar_outside->DrawSprite("frame", position, 0.0f, { 1,1 }, { 1,1,1,1 }, { 0.0f,0.5f });

	const Vector4 color_red = { 0.9f,0.1f,0.2f,1 };
	const Vector4 color_blue = { 0.1f,0.2f,0.9f,1 };
	bar_inside->DrawSprite("white1x1", position, 0.0f, { barSize_x_inside, barSize_max.y }, color_blue, { 0.0f,0.5f });
	bar_inside_sub->DrawSprite("white1x1", position, 0.0f, { barSize_x_inside_sub, barSize_max.y }, color_red, { 0.0f,0.5f });
}

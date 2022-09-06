#include "StockCansBar.h"

StockCansBar::StockCansBar()
{
	bar_outside = new Sprite();
	bar_inside = new Sprite();
	bar_icon = new Sprite();

	cansCountSprite = new NumberSprite(nowCansCount);
}

StockCansBar::~StockCansBar()
{
	delete bar_outside;
	delete bar_inside;
	delete bar_icon;
	delete cansCountSprite;
}

void StockCansBar::Initialize(const float arg_firstCansCount)
{
	firstCansCount = arg_firstCansCount;
	nowCansCount = firstCansCount;
	barinside_sizeX = barSize_max.x;
}

void StockCansBar::Update(const float arg_nowCansCount)
{
	nowCansCount = arg_nowCansCount;

	//í∑Ç≥ïœçX
	if (firstCansCount <= nowCansCount)
	{
		barinside_sizeX = barSize_max.x;
	}
	else
	{
		barinside_sizeX = (barSize_max.x / firstCansCount) * nowCansCount;
	}
}

void StockCansBar::Draw()
{
	const Vector2 position = {100, 150};

	cansCountSprite->Draw(std::to_string((int)nowCansCount).size(), "number", position);

	bar_icon->DrawSprite("can", position, -10.0f, { 0.45f, 0.45f });

	const Vector4 color_red = { 0.9f,0.1f,0.2f,1 };
	bar_inside->DrawSprite("white1x1", position, 0.0f, { barinside_sizeX, barSize_max.y }, color_red, { 0.0f,0.5f });

	bar_outside->DrawSprite("white1x1", position, 0.0f, barSize_max, { 1,1,1,1 }, { 0.0f,0.5f });
}

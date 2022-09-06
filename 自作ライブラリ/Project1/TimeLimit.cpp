#include "TimeLimit.h"
#include "Vector.h"
#include "Easing.h"

TimeLimit::TimeLimit()
{
	int limitFrame = 1;
	for (int i = 0; i < timerDigits; i++)
	{
		limitFrame *= 10;
	}
	limitFrame--;
	limitFrame *= 60;
	timer = new Timer(limitFrame);

	stopwatch_sprite = new Sprite();
	seconds_sprite = new NumberSprite(seconds);
}

TimeLimit::~TimeLimit()
{
	delete timer;
	delete stopwatch_sprite;
	delete seconds_sprite;
}

void TimeLimit::Initialize()
{
	timer->Initialize();
	seconds = 0.0f;
}

void TimeLimit::Update()
{
	//経過時間（秒）
	seconds = timer->GetRealTime(TimerPerformance::Up);

	timer->Update();
}

void TimeLimit::Draw()
{
	//画像サイズ（数字1つ分）
	const Vector2 numberTexSize = { 47.0f, 86.0f };
	//アイコン画像サイズ
	const float stopwatchTexSizeX = 340.0f;
	const float stopwatchScale = 0.25f;

	//画面サイズ
	const Vector2 windowSize = { 1920.0f, 1080.0f };
	//アイコン座標
	Vector2 position = { (windowSize.x / 2) - (stopwatchTexSizeX * stopwatchScale), windowSize.y / 8};
	//アイコン描画
	stopwatch_sprite->DrawSprite("stopwatch", position, 0, { stopwatchScale,stopwatchScale }, { 1,1,1,1 }, { 0.0f,0.5f }, "NoAlphaToCoverageSprite");

	//アイコンの大きさ分ずらす
	position.x += (stopwatchTexSizeX * stopwatchScale) + (numberTexSize.x / 2.0f);
	//数字描画
	seconds_sprite->Draw(timerDigits, "number", position, { 1,1 }, { 1,1,1,1 }, { 0.0f,0.5f }, "NoAlphaToCoverageSprite");
}

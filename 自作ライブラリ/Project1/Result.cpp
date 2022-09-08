#include "Result.h"
#include "Input.h"
#include "Easing.h"

Result::Result()
{
	sp_back = new Sprite();

	canCountSp = new Sprite();
	jumpCountSp = new Sprite();
	canCountNumSp = new NumberSprite(canCount);
	jumpCountNumSp = new NumberSprite(jumpCount);

	timer_display = new Timer(TimerPerformance::Up);

	button = new Sprite();
}

Result::~Result()
{
	delete sp_back;
	delete canCountSp;
	delete jumpCountSp;
	delete canCountNumSp;
	delete jumpCountNumSp;
	delete timer_display;
	delete button;
}

void Result::Initialize()
{
	activeFlag = false;

	step_display = 0;
	timer_display->Initialize();
	timer_display->SetLimit(timerLimit_display, true);

	canCount = 0.0f;
	jumpCount = 0.0f;

	position_can = { positionX_slideIn_start, 500.0f };
	position_jump = { positionX_slideIn_start, 700.0f };

	isSceneChangeStandby = false;
	isToNextScene = false;

	alpha_button = 0.0f;
}

void Result::Update()
{
	if (!activeFlag)
		return;

	//
	SlideIn();

	timer_display->Update();
	//次の表示段階へ
	if (step_display < stepLimit_display)
	{
		if (timer_display->IsTime())
		{
			step_display++;
			timer_display->Reset();
		}
	}
	else
	{
		isSceneChangeStandby = true;
		//
		if (alpha_button < 1.0f)
		{
			const float limit_alpha = 120.0f;
			const float speed_alpha = 1.0f / limit_alpha;
			alpha_button += speed_alpha;
		}
	}

	//シーン切り替え
	if (isSceneChangeStandby &&
		(Input::TriggerPadButton(XINPUT_GAMEPAD_A) || Input::TriggerKey(DIK_SPACE)))
	{
		isToNextScene = true;
	}
}

void Result::Draw()
{
	if (!activeFlag)
		return;

	//段階に分けて順に表示
	if (step_display >= 0)
	{
		canCountNumSp->Draw(std::to_string((int)canCount).size(), "number", position_can, { 1.5f,1.5f });
		canCountSp->DrawSprite("white1x1", position_can, 0.0f, { 512.0f, 128.0f }, { 1,1,1,1 }, { 0.7f,0.5f });
	}
	if (step_display >= 1)
	{
		jumpCountNumSp->Draw(std::to_string((int)jumpCount).size(), "number", position_jump, { 1.5f,1.5f });
		jumpCountSp->DrawSprite("white1x1", position_jump, 0.0f, { 512.0f, 128.0f }, { 1,1,1,1 }, { 0.7f,0.5f });
	}

	//黒背景
	const Vector2 scale = { 1920.0f, 1080.0f };
	const Vector4 bg_color = { 0,0,0,0.3f };
	sp_back->DrawSprite("white1x1", {0.0f,0.0f}, 0.0f, scale, bg_color, { 0.0f,0.0f }, "NoAlphaToCoverageSprite");

	//決定ボタン
	const Vector2 position_button = { 1920.0f - 100.0f, 1080.0f - 100.0f };
	button->DrawSprite("button_a", position_button, 0.0f, { 1,1 }, { 1,1,1,alpha_button }, { 0.5f,0.5f }, "NoAlphaToCoverageSprite");
}

void Result::IsActive(const float arg_canCount, const float arg_jumpCount)
{
	activeFlag = true;
	SetScores(arg_canCount, arg_jumpCount);
}

void Result::SetScores(const float arg_canCount, const float arg_jumpCount)
{
	canCount = arg_canCount;
	jumpCount = arg_jumpCount;
}

void Result::SlideIn()
{
	if (step_display == 0)
	{
		position_can.x = Easing::EaseInCubic(positionX_slideIn_start, positionX_slideIn_end, timerLimit_display, timer_display->GetTime());
	}
	else if (step_display == 1)
	{
		position_jump.x = Easing::EaseInCubic(positionX_slideIn_start, positionX_slideIn_end, timerLimit_display, timer_display->GetTime());
	}
}

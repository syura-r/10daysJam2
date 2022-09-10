#include "Result.h"
#include "Input.h"
#include "Easing.h"
#include "Audio.h"

Result::Result()
{
	sp_back = new Sprite();

	countDisplyer_can = new CountDisplayer();
	countDisplyer_jump = new CountDisplayer();

	timer_display = new Timer(TimerPerformance::Up);

	button = new Sprite();
}

Result::~Result()
{
	delete sp_back;
	delete countDisplyer_can;
	delete countDisplyer_jump;
	delete timer_display;
	delete button;
}

void Result::Initialize()
{
	activeFlag = false;

	step_display = 0;
	timer_display->Initialize();
	timer_display->SetLimit(timerLimit_display, true);

	countDisplyer_can->Initialize(500.0f);
	countDisplyer_jump->Initialize(700.0f);

	isSceneChangeStandby = false;
	isToNextScene = false;

	alpha_button = 0.0f;
}

void Result::Update()
{
	if (!activeFlag)
		return;

	//
	if (step_display == 0)
	{
		countDisplyer_can->SlideIn(timerLimit_display, timer_display->GetTime());
	}
	else if (step_display == 1)
	{
		countDisplyer_jump->SlideIn(timerLimit_display, timer_display->GetTime());
	}

	timer_display->Update();
	//ŽŸ‚Ì•\Ž¦’iŠK‚Ö
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

	//ƒV[ƒ“Ø‚è‘Ö‚¦
	if (isSceneChangeStandby &&
		(Input::TriggerPadButton(XINPUT_GAMEPAD_A) || Input::TriggerKey(DIK_SPACE)))
	{
		Audio::PlaySE("SE_Decision", 1.0f * Audio::volume_se);
		isToNextScene = true;
	}
}

void Result::Draw()
{
	if (!activeFlag)
		return;

	//’iŠK‚É•ª‚¯‚Ä‡‚É•\Ž¦
	if (step_display >= 0)
	{
		countDisplyer_can->Draw();
	}
	if (step_display >= 1)
	{
		countDisplyer_jump->Draw();
	}

	//•”wŒi
	const Vector2 scale = { 1920.0f, 1080.0f };
	const Vector4 bg_color = { 0,0,0,0.3f };
	sp_back->DrawSprite("white1x1", {0.0f,0.0f}, 0.0f, scale, bg_color, { 0.0f,0.0f }, "NoAlphaToCoverageSprite");

	//Œˆ’èƒ{ƒ^ƒ“
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
	countDisplyer_can->count = arg_canCount;
	countDisplyer_jump->count = arg_jumpCount;
}


Result::CountDisplayer::CountDisplayer()
{
	sprite = new Sprite();
	numberSprite = new NumberSprite(count);
}

Result::CountDisplayer::~CountDisplayer()
{
	delete sprite;
	delete numberSprite;
}

void Result::CountDisplayer::Initialize(const float arg_position_y)
{
	count = 0.0f;
	position_sp = { position_slideIn_start_x,arg_position_y };
}

void Result::CountDisplayer::Update()
{
}

void Result::CountDisplayer::Draw()
{
	const Vector2 position_numSp = { position_sp.x, position_sp.y };
	numberSprite->Draw(std::to_string((int)count).size(), "number", position_numSp, { 1.5f,1.5f });

	sprite->DrawSprite("white1x1", position_sp, 0.0f, { 512.0f, 128.0f });
}

void Result::CountDisplayer::SlideIn(const float limitTime, const float nowTime)
{
	position_sp.x = Easing::EaseInCubic(position_slideIn_start_x, position_slideIn_end_x, limitTime, nowTime);
}

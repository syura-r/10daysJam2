#include "Result.h"
#include "Input.h"
#include "Easing.h"
#include "Audio.h"

Result* Result::instance = nullptr;

Result::Result()
{
	sp_back = new Sprite();

	countDisplyer_can = new CountDisplayer();
	countDisplyer_time = new CountDisplayer();

	sprite_clear = new Sprite();

	button = new Sprite();
}

Result::~Result()
{
	delete sp_back;
	delete countDisplyer_can;
	delete countDisplyer_time;
	delete sprite_clear;
	delete button;
}

Result* Result::GetInstance()
{
	if (instance == nullptr)
		instance = new Result();
	return instance;
}

void Result::Finalize()
{
	delete instance;
}

void Result::Initialize()
{
	activeFlag = false;

	step_display = 0;
	timer = 0;

	countDisplyer_can->Initialize(650.0f);
	countDisplyer_time->Initialize(850.0f);

	isClear = false;
	rotation_clear = 0.0f;
	scale_clear = { 1.0f, 1.0f };
	alpha_clear = 1.0f;

	isCloseResult_standby = false;
	isCloseResult = false;

	alpha_button = 0.0f;
}

void Result::Update()
{
	if (!activeFlag)
		return;

	//
	if (step_display == 0)
	{
		if (isClear)//ゲームクリアの時
		{
			//
			rotation_clear = Easing::EaseInCubic(0.0f, 360.0f * 3, timerLimit, timer);
			//
			scale_clear.x = Easing::EaseInCubic(0.1f, 1.0f, timerLimit, timer);
			scale_clear.y = Easing::EaseInCubic(0.1f, 1.0f, timerLimit, timer);
		}
		else//ゲームオーバーの時
		{
			alpha_clear = Easing::EaseInCubic(0.0f, 1.0f, timerLimit, timer);
		}
	}
	else if (step_display == 1)
	{
		countDisplyer_can->SlideIn(timerLimit, timer);
	}
	else if (step_display == 2)
	{
		countDisplyer_time->SlideIn(timerLimit, timer);
	}

	timer++;
	//次の表示段階へ
	if (step_display < stepLimit_display)
	{
		if (timer > timerLimit)
		{
			step_display++;
			timer = 0;
		}
	}
	else
	{
		isCloseResult_standby = true;
		//
		if (alpha_button < 1.0f)
		{
			const float limit_alpha = 120.0f;
			const float speed_alpha = 1.0f / limit_alpha;
			alpha_button += speed_alpha;
		}
	}

	//リザルト終了
	if (isCloseResult_standby &&
		(Input::TriggerPadButton(XINPUT_GAMEPAD_A) || Input::TriggerKey(DIK_SPACE)))
	{
		Audio::PlaySE("decision", 1.0f * Audio::volume_se);
		isCloseResult = true;
		activeFlag = false;
	}

	countDisplyer_can->Update();
	countDisplyer_time->Update();
}

void Result::Draw()
{
	if (!activeFlag)
		return;

	//段階に分けて順に表示

	if (step_display >= 1)
	{
		countDisplyer_can->Draw("life");
	}
	if (step_display >= 2)
	{
		countDisplyer_time->Draw("time");
	}

	//黒背景
	const Vector2 scale = { 1920.0f, 1080.0f };
	const Vector4 bg_color = { 0,0,0,0.3f };
	sp_back->DrawSprite("white1x1", {0.0f,0.0f}, 0.0f, scale, bg_color, { 0.0f,0.0f }, "NoAlphaToCoverageSprite");

	//決定ボタン
	const Vector2 position_button = { 1920.0f - 300.0f, 1080.0f - 100.0f };
	button->DrawSprite("button_a", position_button, 0.0f, { 1,1 }, { 1,1,1,alpha_button }, { 0.5f,0.5f }, "NoAlphaToCoverageSprite");

	//
	if (step_display >= 0)
	{
		std::string texName = "clear";
		if (!isClear)
			texName = "over";
		sprite_clear->DrawSprite(texName, { 1920.0f / 2, 350.0f }, rotation_clear, scale_clear, { 1,1,1,alpha_clear }, { 0.5f,0.5f }, "NoAlphaToCoverageSprite");
	}
}

void Result::SetCanCount(const float arg_canCount)
{
	countDisplyer_can->count = arg_canCount;
}

void Result::SetTimeCount(const float arg_timeCount)
{
	countDisplyer_time->count = arg_timeCount;
}

void Result::IsActive(const bool arg_isClear)
{
	if (!activeFlag)
	{
		Audio::PlaySE("result", 0.1f * Audio::volume_bgm);
	}
	activeFlag = true;

	isClear = arg_isClear;
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
	if (count < 0.0f)
	{
		count = 0.0f;
	}
}

void Result::CountDisplayer::Draw(const std::string& arg_baseTexName)
{
	const Vector2 position_numSp = { position_sp.x + 200.0f, position_sp.y };
	numberSprite->Draw(std::to_string((int)count).size(), "number", position_numSp, { 1,1 }, { 1,1,1,1 }, { 1.0f,0.5f });

	sprite->DrawSprite(arg_baseTexName, position_sp);
}

void Result::CountDisplayer::SlideIn(const float limitTime, const float nowTime)
{
	position_sp.x = Easing::EaseInCubic(position_slideIn_start_x, position_slideIn_end_x, limitTime, nowTime);
}

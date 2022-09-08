#include "Pause.h"
#include"Input.h"
#include "Audio.h"
#include "Easing.h"

bool Pause::fadeFlag = false;

Pause::Pause()
{
	sp_back = new Sprite();
	sp_base = new Sprite();
	toGame = new SelectSprite();
	restart = new SelectSprite();
	toTitle = new SelectSprite();
	bgm = new SelectSprite();
	bar_bgm = new Sprite();
	circle_bgm = new Sprite();
	se = new SelectSprite();
	bar_se = new Sprite();
	circle_se = new Sprite();
}

Pause::~Pause()
{
	delete sp_back;
	delete sp_base;
	delete toGame;
	delete restart;
	delete toTitle;
	delete bgm;
	delete bar_bgm;
	delete circle_bgm;
	delete se;
	delete bar_se;
	delete circle_se;
}

void Pause::Initialize()
{
	activeFlag = false;
	fadeFlag = false;

	selectState = SelectState::ToGame;

	inputInterval_UD = 0;
	inputInterval_LR = 0;

	float pos_y_standard = 1080.0f / (selectMax + 1);
	toGame->Initialize("toGame", pos_y_standard);
	flag_toGame = false;
	restart->Initialize("restart", pos_y_standard * 2);
	flag_restart = false;
	toTitle->Initialize("toTitle", pos_y_standard * 3);
	flag_toTitle = false;

	bgm->Initialize("bgm", pos_y_standard * 4);
	barPositionLeft_bgm = { (1920.0f / 2.0f) - (bar_scale.x / 2.0f),pos_y_standard * 4.5f };
	circlePosition_bgm = barPositionLeft_bgm;

	se->Initialize("se", pos_y_standard * 5);
	barPositionLeft_se = { (1920.0f / 2.0f) - (bar_scale.x / 2.0f),pos_y_standard * 5.5f };
	circlePosition_se = barPositionLeft_se;

	VolumeChange();

	pos_base = toGame->pos;
	alpha_base = 1.0f;
	isUP_alphaChange = false;
}

void Pause::Update()
{
	//ポーズのオンオフ
	if (Input::TriggerPadButton(XINPUT_GAMEPAD_START) || Input::TriggerKey(DIK_ESCAPE))
	{
		activeFlag = !activeFlag;

		if (activeFlag)
		{
			Audio::AllPauseSE();
		}
		else
		{
			Audio::AllResumeSE();
		}

		//Audio::PlaySE("SE_Decision", 1.0f * Audio::volume_se);
		if (!activeFlag)
		{
			fadeFlag = true;
		}
	}

	if (fadeFlag)
	{
		//遷移を入れる時はここに、終わったfalseにする
		fadeFlag = false;
	}

	if (!activeFlag && !fadeFlag)
	{
		Initialize();
		return;
	}

	if (!fadeFlag)
	{
		//選択
		Select();

		//点滅
		const float speed_alphaChange = 0.02f;//速度
		const float min_alphaChange = 0.3f;//下限
		const float max_alphaChange = 1.0f;//上限
		if (isUP_alphaChange)//不透明に
		{
			alpha_base += speed_alphaChange;
			if (alpha_base >= max_alphaChange)
			{
				isUP_alphaChange = !isUP_alphaChange;
			}
		}
		else//透明に
		{
			alpha_base -= speed_alphaChange;
			if (alpha_base <= min_alphaChange)
			{
				isUP_alphaChange = !isUP_alphaChange;
			}
		}
		//決定
		Decision();
	}

	//baseの座標変更
	switch (selectState)
	{
	case ToGame:
		pos_base = toGame->pos;
		break;

	case Restart:
		pos_base = restart->pos;
		break;

	case ToTitle:
		pos_base = toTitle->pos;
		break;

	case BGM:
		pos_base = { bgm->pos.x, barPositionLeft_bgm.y };
		break;

	case SE:
		pos_base = { se->pos.x, barPositionLeft_se.y };
		break;

	default:
		break;
	}

	toGame->Update();
	restart->Update();
	toTitle->Update();
	bgm->Update();
	se->Update();
}

void Pause::Draw()
{
	if (!activeFlag && !fadeFlag)
		return;

	toGame->Draw();
	restart->Draw();
	toTitle->Draw();
	bgm->Draw();
	se->Draw();

	//ボリュームゲージ系
	const Vector2 scale_circle = { 0.7f, 0.7f };
	bar_bgm->DrawSprite("white1x1", barPositionLeft_bgm, 0.0f, bar_scale, { 1,1,1,1 }, { 0.0f,0.5f });
	circle_bgm->DrawSprite("circle", circlePosition_bgm, 0.0f, scale_circle);
	bar_se->DrawSprite("white1x1", barPositionLeft_se, 0.0f, bar_scale, { 1,1,1,1 }, { 0.0f,0.5f });
	circle_se->DrawSprite("circle", circlePosition_se, 0.0f, scale_circle);

	//黒背景
	const XMFLOAT2 scale = { 1920, 1080 };
	const XMFLOAT4 bg_color = { 0,0,0,0.4f };
	sp_back->DrawSprite("white1x1", pos_back, 0.0f, scale, bg_color, { 0.0f,0.0f }, "NoAlphaToCoverageSprite");

	//選択強調
	sp_base->DrawSprite("white1x1", pos_base, 0.0f, { 256.0f, 64.0f }, { 0.3f,0.3f,0.3f,alpha_base }, { 0.5f,0.5f }, "NoAlphaToCoverageSprite");
}

void Pause::Select()
{
	bool isSelectMove = false;//選択を変えたか
	int select = selectState;
	if (Input::CheckPadLStickUp() || Input::DownKey(DIK_W))
	{
		if (inputInterval_UD % inputInterval == 0)
		{
			//
			if (selectState > 0)
			{
				isSelectMove = true;
				select--;
			}
			//
		}
		inputInterval_UD++;
	}
	else if (Input::CheckPadLStickDown() || Input::DownKey(DIK_S))
	{
		if (inputInterval_UD % inputInterval == 0)
		{
			//
			if (selectState < selectMax - 1)
			{
				isSelectMove = true;
				select++;
			}
			//
		}
		inputInterval_UD++;
	}
	else
	{
		inputInterval_UD = 0;
	}
	selectState = (SelectState)select;

	//カーソル位置
	if (isSelectMove)
	{
		//透明度リセット
		alpha_base = 1.0f;
		isUP_alphaChange = false;
		//Audio::PlaySE("SE_Select", 1.0f * Audio::volume_se);
	}


	//音量設定の項目かどうか
	if (selectState == SelectState::BGM ||
		selectState == SelectState::SE)
	{
		//音量設定
		VolumeChange();
	}

}

void Pause::Decision()
{
	if (Input::TriggerPadButton(XINPUT_GAMEPAD_A) || Input::TriggerKey(DIK_SPACE))
	{
		//Audio::PlaySE("SE_Decision", 1.0f * Audio::volume_se);

		switch (selectState)
		{
		case ToGame:
			//ゲームにもどる
			flag_toGame = true;
			//設定を閉じる
			activeFlag = false;
			break;

		case Restart:
			//やり直し
			flag_restart = true;
			//設定を閉じる
			activeFlag = false;
			break;

		case ToTitle:
			//タイトルにもどる
			flag_toTitle = true;
			//設定を閉じる
			activeFlag = false;
			break;

		case BGM:
			break;
		case SE:
			break;
		default:
			break;
		}
	}
}

void Pause::VolumeChange()
{
	//各設定モード
	const float volumeMax = 2.0f;
	const float volumeOneScale = 0.1f;
	float stock_volume_bgm = Audio::volume_bgm;
	float stock_volume_se = Audio::volume_se;
	switch (selectState)
	{
	case BGM:
		//音量変更
		if (Input::CheckPadLStickLeft() || Input::DownKey(DIK_A))
		{
			if (inputInterval_LR % inputInterval == 0)
			{
				//
				if (stock_volume_bgm > 0.0f)
				{
					stock_volume_bgm -= volumeOneScale;
				}
				//
			}
			inputInterval_LR++;
		}
		else if (Input::CheckPadLStickRight() || Input::DownKey(DIK_D))
		{
			if (inputInterval_LR % inputInterval == 0)
			{
				//
				if (volumeMax > stock_volume_bgm)
				{
					stock_volume_bgm += volumeOneScale;
				}
				//
			}
			inputInterval_LR++;
		}
		else
		{
			inputInterval_LR = 0;
		}
		break;

	case SE:
		//音量変更
		if (Input::CheckPadLStickLeft() || Input::DownKey(DIK_A))
		{
			if (inputInterval_LR % inputInterval == 0)
			{
				//
				if (stock_volume_se > 0.0f)
				{
					stock_volume_se -= volumeOneScale;
				}
				//
			}
			inputInterval_LR++;
		}
		else if (Input::CheckPadLStickRight() || Input::DownKey(DIK_D))
		{
			if (inputInterval_LR % inputInterval == 0)
			{
				//
				if (volumeMax > stock_volume_se)
				{
					stock_volume_se += volumeOneScale;
				}
				//
			}
			inputInterval_LR++;
		}
		else
		{
			inputInterval_LR = 0;
		}
		break;

	default:
		break;
	}
	//丸の位置変更
	circlePosition_bgm.x = barPositionLeft_bgm.x + (stock_volume_bgm * (bar_scale.x / volumeMax));
	circlePosition_se.x = barPositionLeft_se.x + (stock_volume_se * (bar_scale.x / volumeMax));

	//audioの変数に反映
	Audio::volume_bgm = stock_volume_bgm;
	Audio::volume_se = stock_volume_se;
}

Pause::SelectSprite::SelectSprite()
{
	sprite = new Sprite();
}

Pause::SelectSprite::~SelectSprite()
{
	delete sprite;
}

void Pause::SelectSprite::Initialize(const std::string& texName, const float posY)
{
	this->texName = texName;
	pos = { 1920.0f / 2.0f, posY };
}

void Pause::SelectSprite::Update()
{
}

void Pause::SelectSprite::Draw()
{
	sprite->DrawSprite(texName, pos);
}

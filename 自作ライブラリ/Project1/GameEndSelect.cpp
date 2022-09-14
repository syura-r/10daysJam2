#include "GameEndSelect.h"
#include "Input.h"
#include "Audio.h"

GameEndSelect::GameEndSelect()
{
	sp_back = new Sprite();
	sp_base = new Sprite();
	restart = new SelectSprite_inGameEndSelect();
	toTitle = new SelectSprite_inGameEndSelect();
}

GameEndSelect::~GameEndSelect()
{
	delete sp_back;
	delete sp_base;
	delete restart;
	delete toTitle;
}

void GameEndSelect::Initialize()
{
	activeFlag = false;

	selectState = SelectState::Restart;

	float pos_y_standard = 1080.0f / (selectMax + 1);
	restart->Initialize("restart", pos_y_standard);
	flag_restart = false;
	toTitle->Initialize("toTitle", pos_y_standard * 2);
	flag_toTitle = false;

	pos_base = restart->pos;
	alpha_base = 1.0f;
	isUP_alphaChange = false;
}

void GameEndSelect::Update()
{
	if (!activeFlag)
	{
		return;
	}

	//選択
	Select();

	//点滅
	const float speed_alphaChange = 0.02f;//速度
	const float min_alphaChange = 0.1f;//下限
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

	//baseの座標変更
	switch (selectState)
	{
	case Restart:
		pos_base = restart->pos;
		break;

	case ToTitle:
		pos_base = toTitle->pos;
		break;

	default:
		break;
	}

	restart->Update();
	toTitle->Update();
}

void GameEndSelect::Draw()
{
	if (!activeFlag)
	{
		return;
	}

	restart->Draw();
	toTitle->Draw();

	//黒背景
	const XMFLOAT2 scale = { 1920, 1080 };
	const XMFLOAT4 bg_color = { 0,0,0,0.4f };
	sp_back->DrawSprite("white1x1", pos_back, 0.0f, scale, bg_color, { 0.0f,0.0f }, "NoAlphaToCoverageSprite");

	//選択強調
	sp_base->DrawSprite("select", pos_base, 0.0f, { 1.2f, 1.0f }, { 1,1,1,alpha_base }, { 0.5f,0.5f }, "NoAlphaToCoverageSprite");
}

void GameEndSelect::IsActive()
{
	activeFlag = true;
}

void GameEndSelect::Select()
{
	bool isSelectMove = false;//選択を変えたか
	int select = selectState;
	if (Input::CheckPadLStickUp() || Input::DownKey(DIK_W))
	{
			if (selectState > 0)
			{
				isSelectMove = true;
				select--;
			}
	}
	else if (Input::CheckPadLStickDown() || Input::DownKey(DIK_S))
	{
			if (selectState < selectMax - 1)
			{
				isSelectMove = true;
				select++;
			}
	}
	selectState = (SelectState)select;

	//カーソル位置
	if (isSelectMove)
	{
		//透明度リセット
		alpha_base = 1.0f;
		isUP_alphaChange = false;
		Audio::PlaySE("select", 1.0f * Audio::volume_se);
	}
}

void GameEndSelect::Decision()
{
	if (Input::TriggerPadButton(XINPUT_GAMEPAD_A) || Input::TriggerKey(DIK_SPACE))
	{
		Audio::PlaySE("decision", 1.0f * Audio::volume_se);

		switch (selectState)
		{
		case Restart:
			//やり直し
			flag_restart = true;
			//閉じる
			activeFlag = false;
			break;

		case ToTitle:
			//タイトルにもどる
			flag_toTitle = true;
			//閉じる
			activeFlag = false;
			break;

		default:
			break;
		}
	}
}

GameEndSelect::SelectSprite_inGameEndSelect::SelectSprite_inGameEndSelect()
{
	sprite = new Sprite();
}

GameEndSelect::SelectSprite_inGameEndSelect::~SelectSprite_inGameEndSelect()
{
	delete sprite;
}

void GameEndSelect::SelectSprite_inGameEndSelect::Initialize(const std::string& texName, const float posY)
{
	this->texName = texName;
	pos = { 1920.0f / 2.0f, posY };
}

void GameEndSelect::SelectSprite_inGameEndSelect::Update()
{
}

void GameEndSelect::SelectSprite_inGameEndSelect::Draw()
{
	sprite->DrawSprite(texName, pos);
}

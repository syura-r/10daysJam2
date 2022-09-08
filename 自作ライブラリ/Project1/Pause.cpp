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
	//�|�[�Y�̃I���I�t
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
		//�J�ڂ����鎞�͂����ɁA�I�����false�ɂ���
		fadeFlag = false;
	}

	if (!activeFlag && !fadeFlag)
	{
		Initialize();
		return;
	}

	if (!fadeFlag)
	{
		//�I��
		Select();

		//�_��
		const float speed_alphaChange = 0.02f;//���x
		const float min_alphaChange = 0.3f;//����
		const float max_alphaChange = 1.0f;//���
		if (isUP_alphaChange)//�s������
		{
			alpha_base += speed_alphaChange;
			if (alpha_base >= max_alphaChange)
			{
				isUP_alphaChange = !isUP_alphaChange;
			}
		}
		else//������
		{
			alpha_base -= speed_alphaChange;
			if (alpha_base <= min_alphaChange)
			{
				isUP_alphaChange = !isUP_alphaChange;
			}
		}
		//����
		Decision();
	}

	//base�̍��W�ύX
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

	//�{�����[���Q�[�W�n
	const Vector2 scale_circle = { 0.7f, 0.7f };
	bar_bgm->DrawSprite("white1x1", barPositionLeft_bgm, 0.0f, bar_scale, { 1,1,1,1 }, { 0.0f,0.5f });
	circle_bgm->DrawSprite("circle", circlePosition_bgm, 0.0f, scale_circle);
	bar_se->DrawSprite("white1x1", barPositionLeft_se, 0.0f, bar_scale, { 1,1,1,1 }, { 0.0f,0.5f });
	circle_se->DrawSprite("circle", circlePosition_se, 0.0f, scale_circle);

	//���w�i
	const XMFLOAT2 scale = { 1920, 1080 };
	const XMFLOAT4 bg_color = { 0,0,0,0.4f };
	sp_back->DrawSprite("white1x1", pos_back, 0.0f, scale, bg_color, { 0.0f,0.0f }, "NoAlphaToCoverageSprite");

	//�I������
	sp_base->DrawSprite("white1x1", pos_base, 0.0f, { 256.0f, 64.0f }, { 0.3f,0.3f,0.3f,alpha_base }, { 0.5f,0.5f }, "NoAlphaToCoverageSprite");
}

void Pause::Select()
{
	bool isSelectMove = false;//�I����ς�����
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

	//�J�[�\���ʒu
	if (isSelectMove)
	{
		//�����x���Z�b�g
		alpha_base = 1.0f;
		isUP_alphaChange = false;
		//Audio::PlaySE("SE_Select", 1.0f * Audio::volume_se);
	}


	//���ʐݒ�̍��ڂ��ǂ���
	if (selectState == SelectState::BGM ||
		selectState == SelectState::SE)
	{
		//���ʐݒ�
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
			//�Q�[���ɂ��ǂ�
			flag_toGame = true;
			//�ݒ�����
			activeFlag = false;
			break;

		case Restart:
			//��蒼��
			flag_restart = true;
			//�ݒ�����
			activeFlag = false;
			break;

		case ToTitle:
			//�^�C�g���ɂ��ǂ�
			flag_toTitle = true;
			//�ݒ�����
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
	//�e�ݒ胂�[�h
	const float volumeMax = 2.0f;
	const float volumeOneScale = 0.1f;
	float stock_volume_bgm = Audio::volume_bgm;
	float stock_volume_se = Audio::volume_se;
	switch (selectState)
	{
	case BGM:
		//���ʕύX
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
		//���ʕύX
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
	//�ۂ̈ʒu�ύX
	circlePosition_bgm.x = barPositionLeft_bgm.x + (stock_volume_bgm * (bar_scale.x / volumeMax));
	circlePosition_se.x = barPositionLeft_se.x + (stock_volume_se * (bar_scale.x / volumeMax));

	//audio�̕ϐ��ɔ��f
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

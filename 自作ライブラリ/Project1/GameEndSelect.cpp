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

	//�I��
	Select();

	//�_��
	const float speed_alphaChange = 0.02f;//���x
	const float min_alphaChange = 0.1f;//����
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

	//base�̍��W�ύX
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

	//���w�i
	const XMFLOAT2 scale = { 1920, 1080 };
	const XMFLOAT4 bg_color = { 0,0,0,0.4f };
	sp_back->DrawSprite("white1x1", pos_back, 0.0f, scale, bg_color, { 0.0f,0.0f }, "NoAlphaToCoverageSprite");

	//�I������
	sp_base->DrawSprite("select", pos_base, 0.0f, { 1.2f, 1.0f }, { 1,1,1,alpha_base }, { 0.5f,0.5f }, "NoAlphaToCoverageSprite");
}

void GameEndSelect::IsActive()
{
	activeFlag = true;
}

void GameEndSelect::Select()
{
	bool isSelectMove = false;//�I����ς�����
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

	//�J�[�\���ʒu
	if (isSelectMove)
	{
		//�����x���Z�b�g
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
			//��蒼��
			flag_restart = true;
			//����
			activeFlag = false;
			break;

		case ToTitle:
			//�^�C�g���ɂ��ǂ�
			flag_toTitle = true;
			//����
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

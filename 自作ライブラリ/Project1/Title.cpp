#include "Title.h"
#include "Easing.h"
//#include "Audio.h"

Title::Title()
{
	next = Play;

	camera = std::make_unique<InGameCamera>();
	Object3D::SetCamera(camera.get());

	//���C�g����
	lightGroup.reset(LightGroup::Create());
	//3D�I�u�W�F�N�g�Ƀ��C�g���Z�b�g
	Object3D::SetLightGroup(lightGroup.get());
	//���C�g�F��ݒ�
	lightGroup->SetDirLightActive(0, true);
	lightGroup->SetDirLightColor(0, { 1,1,1 });

	logo = new Sprite();
	base = new Sprite();
	start = new Sprite();
	quit = new Sprite();
}


Title::~Title()
{
	delete logo;
	delete base;
	delete start;
	delete quit;
}

void Title::Initialize()
{
	isEnd = false;
	isAllEnd = false;

	Object3D::SetCamera(camera.get());
	Object3D::SetLightGroup(lightGroup.get());

	position_logo = { positionOrigin_logo.x, positionOrigin_logo.y };
	scale_start = scale_small;
	scale_quit = scale_small;

	actoinStep_logo = 0;
	easingCount_position = 0;

	angle_target = 330.0f;
	const float radian_target = DirectX::XMConvertToRadians(angle_target);
	position_target.x = cos(radian_target) * radius_target + positionOrigin_logo.x;
	position_target.y = sin(radian_target) * radius_target + positionOrigin_logo.y;

	position_base = position_start;
	alpha_base = 1.0f;
	isUP_alphaChange = false;

	selectNumber = 0;
	easingCount_scale = easingCountLimit_scale;

	Input::Update();
}

void Title::Update()
{
	if (Input::TriggerPadButton(XINPUT_GAMEPAD_A) || Input::TriggerKey(DIK_SPACE))
	{
		if (selectNumber <= 0)
		{
			//Play�V�[����
			next = Play;
			ShutDown();
		}
		else if (selectNumber <= 1)
		{
			//�Q�[�����I������
			GameClose();
		}
	}

	Select();
	AlphaChange_Base();

	//���S�̍s��
	switch (actoinStep_logo)
	{
	case 0:
		MoveStraight_Logo();
		break;
	case 1:
		MoveCircle_Logo();
		break;
	default:
		break;
	}

	//
	camera->Update();
	lightGroup->SetAmbientColor({ 1,1,1 });
	lightGroup->SetDirLightDir(0, { 0.0f,-1.0f,0.5f,1 });
	lightGroup->Update();
}

void Title::PreDraw()
{
	//PipelineState::SetPipeline("BasicObj");
}

void Title::PostDraw()
{
	logo->DrawSprite("logo", position_logo);
	start->DrawSprite("start", position_start, 0.0f, scale_start);
	quit->DrawSprite("quit", position_quit, 0.0f, scale_quit);

	base->DrawSprite("white1x1", position_base, 0.0f, { 256.0f, 64.0f }, { 0.3f,0.3f,0.3f,alpha_base }, { 0.5f,0.5f }, "NoAlphaToCoverageSprite");
}

void Title::MoveStraight_Logo()
{
	if (easingCount_position > easingCountLimit_position)
	{
		actoinStep_logo++;
		return;
	}

	position_logo.x = Easing::EaseOutBounce(positionFirst_logo.x, positionOrigin_logo.x, easingCountLimit_position, easingCount_position);
	position_logo.y = Easing::EaseOutBounce(positionFirst_logo.y, positionOrigin_logo.y, easingCountLimit_position, easingCount_position);

	easingCount_position++;
}

void Title::MoveCircle_Logo()
{
	//position_target�����񂳂���
	const float speed_rot = 1.0f;
	angle_target += speed_rot;
	if (angle_target >= 360.0f)
	{
		angle_target = 0.0f;
	}

	const float radian_target = DirectX::XMConvertToRadians(angle_target);
	position_target.x = cos(radian_target) * radius_target + positionOrigin_logo.x;
	position_target.y = sin(radian_target) * radius_target + positionOrigin_logo.y;

	//position_logo��position_target��Ǐ]������
	const float speed_homing = 0.5f;
	Vector2 velocity = position_target - position_logo;
	position_logo += velocity.Normalize(velocity) * speed_homing;
}

void Title::Select()
{
	//�ő區�ڐ�
	const int maxSelectNumber = 1;
	const int selectNumber_stock = selectNumber;

	//���͎�t
	if (Input::TriggerPadLStickUp() || Input::TriggerKey(DIK_W) &&
		selectNumber > 0)
	{
		selectNumber--;
	}
	else if (Input::TriggerPadLStickDown() || Input::TriggerKey(DIK_S) &&
		selectNumber < maxSelectNumber)
	{
		selectNumber++;
	}

	//�ς������
	if (selectNumber_stock != selectNumber)
	{
		alpha_base = 1.0f;
		isUP_alphaChange = false;
		//Audio::PlaySE("SE_Select", 1.0f * Audio::volume_se);
		easingCount_scale = 0;
	}

	//�g�k�ƈړ�
	if (selectNumber <= 0)
	{
		scale_start.x = Easing::EaseInCubic(scale_small.x, scale_big.x, easingCountLimit_scale, easingCount_scale);
		scale_start.y = Easing::EaseInCubic(scale_small.y, scale_big.y, easingCountLimit_scale, easingCount_scale);

		scale_quit.x = Easing::EaseInCubic(scale_big.x, scale_small.x, easingCountLimit_scale, easingCount_scale);
		scale_quit.y = Easing::EaseInCubic(scale_big.y, scale_small.y, easingCountLimit_scale, easingCount_scale);

		position_base = position_start;
	}
	else if(selectNumber >= 1)
	{
		scale_start.x = Easing::EaseInCubic(scale_big.x, scale_small.x, easingCountLimit_scale, easingCount_scale);
		scale_start.y = Easing::EaseInCubic(scale_big.y, scale_small.y, easingCountLimit_scale, easingCount_scale);

		scale_quit.x = Easing::EaseInCubic(scale_small.x, scale_big.x, easingCountLimit_scale, easingCount_scale);
		scale_quit.y = Easing::EaseInCubic(scale_small.y, scale_big.y, easingCountLimit_scale, easingCount_scale);

		position_base = position_quit;
	}

	if (easingCountLimit_scale > easingCount_scale)
	{
		easingCount_scale++;
	}
}

void Title::AlphaChange_Base()
{
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
}

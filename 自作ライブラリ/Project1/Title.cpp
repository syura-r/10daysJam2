#include "Title.h"
#include "Easing.h"
#include "Audio.h"

Title::Title()
{
	next = Play;

	camera = std::make_unique<InGameCamera>();
	Object3D::SetCamera(camera.get());

	//ライト生成
	lightGroup.reset(LightGroup::Create());
	//3Dオブジェクトにライトをセット
	Object3D::SetLightGroup(lightGroup.get());
	//ライト色を設定
	lightGroup->SetDirLightActive(0, true);
	lightGroup->SetDirLightColor(0, { 1,1,1 });

	logo = new Sprite();
	base = new Sprite();
	start = new Sprite();
	quit = new Sprite();
	button = new Sprite();
}


Title::~Title()
{
	delete logo;
	delete base;
	delete start;
	delete quit;
	delete button;
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

	angle_target = angleGap_target;
	const float radian_target = DirectX::XMConvertToRadians(angle_target);
	radius_target = radiusMax_target;
	position_target.x = cos(radian_target) * radius_target + positionOrigin_logo.x;
	position_target.y = sin(radian_target) * radius_target + positionOrigin_logo.y;

	position_base = position_start;
	alpha_base = 1.0f;
	isUP_alphaChange = false;

	selectNumber = 0;
	easingCount_scale = easingCountLimit_scale;

	alpha_button = 0.0f;

	Input::Update();
}

void Title::Update()
{
	//ロゴの行動
	if (actoinStep_logo == 0)
	{
		MoveStraight_Logo();
	}
	else
	{
		MoveCircle_Logo();

		//
		if (alpha_button < 1.0f)
		{
			const float limit_alpha = 120.0f;
			const float speed_alpha = 1.0f / limit_alpha;
			alpha_button += speed_alpha;
		}
	}

	if ((Input::TriggerPadButton(XINPUT_GAMEPAD_A) || Input::TriggerKey(DIK_SPACE)) &&
		actoinStep_logo > 0)
	{
		Audio::PlaySE("SE_Decision", 1.0f * Audio::volume_se);

		if (selectNumber <= 0)
		{
			//Playシーンへ
			next = Play;
			ShutDown();
		}
		else if (selectNumber <= 1)
		{
			//ゲームを終了する
			GameClose();
		}
	}

	Select();
	AlphaChange_Base();

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

	button->DrawSprite("button_a", position_button, 0.0f, { 1,1 }, { 1,1,1,alpha_button }, { 0.5f,0.5f }, "NoAlphaToCoverageSprite");
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
	//position_targetを周回させる
	const float speed_angle = 1.0f;
	angle_target += speed_angle;
	if (angle_target >= 360.0f)
	{
		angle_target = 0.0f;
	}

	//角度に応じて、半径を変える
	const float speed_radius = (radiusMax_target - radiusMin_target) / (360.0f / speed_angle);
	if (angle_target >= 0.0f - angleGap_target && angle_target < 90.0f - angleGap_target ||
		(angle_target >= 180.0f - angleGap_target && angle_target < 270.0f - angleGap_target))
	{
		radius_target -= speed_radius;
	}
	if ((angle_target >= 90.0f - angleGap_target && angle_target < 180.0f - angleGap_target) ||
		(angle_target >= 270.0f - angleGap_target && angle_target < 360.0f - angleGap_target))
	{
		radius_target += speed_radius;
	}


	const float radian_target = DirectX::XMConvertToRadians(angle_target);
	position_target.x = cos(radian_target) * radius_target + positionOrigin_logo.x;
	position_target.y = sin(radian_target) * radius_target + positionOrigin_logo.y;

	//position_logoにposition_targetを追従させる
	const float speed_homing = 0.5f;
	Vector2 velocity = position_target - position_logo;
	position_logo += velocity.Normalize(velocity) * speed_homing;
}

void Title::Select()
{
	//最大項目数
	const int maxSelectNumber = 1;
	const int selectNumber_stock = selectNumber;

	//入力受付
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

	//変わったか
	if (selectNumber_stock != selectNumber)
	{
		alpha_base = 1.0f;
		isUP_alphaChange = false;
		Audio::PlaySE("SE_Select", 1.0f * Audio::volume_se);
		easingCount_scale = 0;
	}

	//拡縮と移動
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
}

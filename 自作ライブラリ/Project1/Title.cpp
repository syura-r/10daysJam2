#include "Title.h"
#include "OBJLoader.h"
#include "Easing.h"
#include "Audio.h"

Title::Title()
{
	next = Play;

	camera = std::make_unique<DebugCamera>();
	Object3D::SetCamera(camera.get());

	//ライト生成
	lightGroup.reset(LightGroup::Create());
	//3Dオブジェクトにライトをセット
	Object3D::SetLightGroup(lightGroup.get());
	//ライト色を設定
	lightGroup->SetDirLightActive(0, true);
	lightGroup->SetDirLightColor(0, { 1,1,1 });


	cans[0] = new CanInTitle(-4.5f, 10.0f, -2.5f, 90.0f, 90.0f);
	cans[1] = new CanInTitle(3.7f, 10.0f, -2.8f, 90.0f, 97.0f);
	cover = Object3D::Create(OBJLoader::GetModel("cover_title"), position_cover, scale_cover, rotation_cover, color_cover);

	base = new Sprite();
	start = new Sprite();
	quit = new Sprite();
	button = new Sprite();
}


Title::~Title()
{
	for (int i = 0; i < cansNum; i++)
	{
		delete cans[i];
	}
	delete cover;
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

	for (int i = 0; i < cansNum; i++)
	{
		cans[i]->Initialize();
	}

	position_start = { position_x_bottom, 800.0f };
	position_quit = { position_x_bottom, 950.0f };
	scale_start = scale_small;
	scale_quit = scale_small;

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
	bool isCanMoveEnd = true;
	for (int i = 0; i < cansNum; i++)
	{
		cans[i]->Update();

		isCanMoveEnd = cans[i]->GetIsMoveEnd() && isCanMoveEnd;
	}

	if (alpha_button < 1.0f)
	{
		const float limit_alpha = 120.0f;
		const float speed_alpha = 1.0f / limit_alpha;
		alpha_button += speed_alpha;
	}

	if ((Input::TriggerPadButton(XINPUT_GAMEPAD_A) || Input::TriggerKey(DIK_SPACE)) &&
		isCanMoveEnd)
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
	cover->Update();

	camera->Update();
	lightGroup->SetAmbientColor({ 1,1,1 });
	lightGroup->SetDirLightDir(0, { 0.0f,-1.0f,0.5f,1 });
	lightGroup->Update();
}

void Title::PreDraw()
{
	PipelineState::SetPipeline("BasicObj");
	for (int i = 0; i < cansNum; i++)
	{
		cans[i]->Draw();
	}
	cover->Draw();
}

void Title::PostDraw()
{
	start->DrawSprite("start", position_start, 0.0f, scale_start);
	quit->DrawSprite("quit", position_quit, 0.0f, scale_quit);

	const Vector2 size_base = { 256.0f * scale_big.x, 64.0f * scale_big.y };
	base->DrawSprite("white1x1", position_base, 0.0f, size_base, { 0.3f,0.3f,0.3f,alpha_base }, { 0.5f,0.5f }, "NoAlphaToCoverageSprite");


	//button->DrawSprite("button_a", position_button, 0.0f, { 1,1 }, { 1,1,1,alpha_button }, { 0.5f,0.5f }, "NoAlphaToCoverageSprite");
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
		position_start.x = Easing::EaseInCubic(position_x_bottom, position_x_top, easingCountLimit_scale, easingCount_scale);
		position_quit.x = Easing::EaseInCubic(position_x_top, position_x_bottom, easingCountLimit_scale, easingCount_scale);

		scale_start.x = Easing::EaseInCubic(scale_small.x, scale_big.x, easingCountLimit_scale, easingCount_scale);
		scale_start.y = Easing::EaseInCubic(scale_small.y, scale_big.y, easingCountLimit_scale, easingCount_scale);

		scale_quit.x = Easing::EaseInCubic(scale_big.x, scale_small.x, easingCountLimit_scale, easingCount_scale);
		scale_quit.y = Easing::EaseInCubic(scale_big.y, scale_small.y, easingCountLimit_scale, easingCount_scale);

		position_base = position_start;
	}
	else if (selectNumber >= 1)
	{
		position_start.x = Easing::EaseInCubic(position_x_top, position_x_bottom, easingCountLimit_scale, easingCount_scale);
		position_quit.x = Easing::EaseInCubic(position_x_bottom, position_x_top, easingCountLimit_scale, easingCount_scale);

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

#include "Play.h"
#include <fstream>
#include <sstream>
#include "Object3D.h"
#include "Audio.h"
#include "ParticleEmitter.h"
#include "ParticleManager.h"
#include "DrawMode.h"
#include "imgui.h"
#include "TouchAbleObject.h"
#include "FBXManager.h"
#include "Input.h"
#include "MapBox.h"
#include "Easing.h"
#include "LevelEditor.h"
#include "Player.h"
#include "Ending.h"
#include "TextureResource.h"
#include "PtrDelete.h"
#include "ScreenCamera.h"
#include "InGameCamera.h"
#include "Result.h"

#include"AreaEffect.h"
#include"Sprite3D.h"
#include "BadGuy.h"
#include "Crow.h"
#include "MapLoader.h"
#include "HitStop.h"
#include "BreakBlock.h"
#include"Apartment.h"
#include "EndChara.h"
#include "Boss.h"
Play::Play()
{
	next = Title;
	camera = std::make_unique<InGameCamera>();
	Object3D::SetCamera(camera.get());
	Sprite3D::SetCamera(camera.get());

	
	//ParticleEmitter::Initialize(camera.get());
	ParticleManager::GetInstance()->SetCamera(camera.get());
	//Player::SetCamera(camera.get());
	//���C�g����
	lightGroup.reset(LightGroup::Create());
	//3D�I�u�W�F�N�g�Ƀ��C�g���Z�b�g
	Object3D::SetLightGroup(lightGroup.get());
	//���C�g�F��ݒ�
	lightGroup->SetDirLightActive(0, true);
	lightGroup->SetDirLightColor(0, XMFLOAT3(color0));
	collisionManager = CollisionManager::GetInstance();
	objectManager = ObjectManager::GetInstance();

	objectManager->AddObjectsAtOnce();

	pause = new Pause();
	Result::GetInstance();
	inGameTimer = new InGameTimer();
	gameEndSelect = new GameEndSelect();

	ParticleEmitter::SetObjectManager(objectManager);
}


Play::~Play()
{
	LevelEditor::GetInstance()->Clear();
	PtrDelete(pause);
	Result::GetInstance()->Finalize();
	PtrDelete(inGameTimer);
	PtrDelete(gameEndSelect);
	ParticleManager::GetInstance()->ClearDeadEffect();
}

void Play::Initialize()
{
	next = Title;

	Object3D::SetCamera(camera.get());
	Object3D::SetLightGroup(lightGroup.get());

	objectManager->Reset();

	operate = false;
	player = new Player(Vector3(0, -5, 0));
	objectManager->Add(player);	
	
	Boss* boss = new Boss();
	objectManager->Add(boss);

	BaseEnemy::SetPlayerPtr(player);
	camera->SetFocusObject(player);
	EndChara* endChara = new EndChara();
	objectManager->Add(endChara);
	camera->SetMoveFocusObject(endChara,player);

	MapLoader::LoadMap("1-4");
	camera->Initialize();

	isEnd = false;
	isAllEnd = false;

	pause->Initialize();
	Result::GetInstance()->Initialize();
	inGameTimer->Initialize();
	gameEndSelect->Initialize();

	nowPlayingBGMName = "play";
	Audio::StopBGM(nowPlayingBGMName);
	Audio::PlayBGM(nowPlayingBGMName, 0.1f * Audio::volume_bgm);


	Input::Update();
	Update();

	ParticleManager::GetInstance()->ClearDeadEffect();
}

void Play::Update()
{
	//���U���g////////////////////////////////
	Result* result = Result::GetInstance();
	result->Update();
	//���U���g�����
	if (result->GetIsCloseResult() && !gameEndSelect->GetIsActive())
	{
		gameEndSelect->IsActive();
		return;
	}
	//
	if (result->GetActive())
	{
		Audio::StopBGM(nowPlayingBGMName);
		return;
	}

	//���U���g��////////////////////////////////
	gameEndSelect->Update();
	//��蒼��
	if (gameEndSelect->GetRestart())
	{
		Audio::StopBGM(nowPlayingBGMName);
		Initialize();
		return;
	}
	//�^�C�g���ɂ��ǂ�
	if (gameEndSelect->GetToTitle())
	{
		Audio::StopBGM(nowPlayingBGMName);
		Audio::AllStopSE();
		next = Title;
		ShutDown();
		return;
	}
	if (gameEndSelect->GetIsActive())
	{
		return;
	}

	//�|�[�Y////////////////////////////////
	pause->Update();
	//�Q�[���ɂ��ǂ�
	if (pause->GetToGame())
	{
		return;
	}
	//��蒼��
	if (pause->GetRestart())
	{
		Audio::StopBGM(nowPlayingBGMName);
		Initialize();
		return;
	}
	//�^�C�g���ɂ��ǂ�
	if (pause->GetToTitle())
	{
		Audio::StopBGM(nowPlayingBGMName);
		Audio::AllStopSE();
		next = Title;
		ShutDown();
		return;
	}
	//�|�[�Y��ʂ��J���Ă���Ƃ�
	if (pause->GetActivePause())
	{
		//BGM�̉��ʕύX
		Audio::VolumeChangeBGM(nowPlayingBGMName, 0.1f * Audio::volume_bgm);
		return;
	}


#ifdef _DEBUG
	if (Input::TriggerKey(DIK_RETURN))
	{
		Crow* crow = new Crow();
		objectManager->Add(crow);
	}
	//�I������
	if (Input::TriggerKey(DIK_E))
	{
		Audio::StopBGM(nowPlayingBGMName);
		Audio::AllStopSE();
		ShutDown();
		return;
	}
#endif
	
	//�^�C�}�[////////////////////////////////
	if(operate)
	inGameTimer->Update();
	result->SetTimeCount(inGameTimer->GetRealTime());

	lightGroup->SetAmbientColor(XMFLOAT3(coloramb));
	lightGroup->SetDirLightDir(0, { lightDir[0],lightDir[1],lightDir[2],1 });
	lightGroup->Update();

	if (!HitStop::Stop())
		objectManager->Update();
	camera->Update();
	if (!camera->GetMoveFocusObject() && !operate)
	{
		operate = true;
		player->CanOperate();
	}
	ParticleManager::GetInstance()->UpdateDeadEffect();
}

void Play::PreDraw()
{

	objectManager->DrawReady();
#ifdef _DEBUG
	if (DrawMode::GetDrawImGui() && !Object3D::GetDrawShadow())
	{
		ImGui::Begin("Light");
		ImGui::SliderFloat3("LightDir", lightDir, -1.0f, 1.0f);

		ImGui::End();
		Object3D::GetLightCamera()->SetLightDir({ lightDir[0],lightDir[1] ,lightDir[2] });
		LevelEditor::GetInstance()->Draw();
	}
#endif
		objectManager->PreDraw();
		AreaEffect::AreaEffectsDraw();
		ParticleManager::GetInstance()->DrawDeadEffect();
}

void Play::PostDraw()
{
	//if (migrate)
	//	return;
	inGameTimer->Draw();

	objectManager->PostDraw();
	if (!Object3D::GetDrawShadow())
	{
		DirectXLib::GetInstance()->DepthClear();
	}

	pause->Draw();
	Result::GetInstance()->Draw();
	gameEndSelect->Draw();
}


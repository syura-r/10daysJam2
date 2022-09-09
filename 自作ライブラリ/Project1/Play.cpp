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

#include "AreaEffect.h"
#include "Sprite3D.h"

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
	result = new Result();
	inGameTimer = new InGameTimer();
	cansBar = new StockCansBar();

	ParticleEmitter::SetObjectManager(objectManager);
}


Play::~Play()
{
	LevelEditor::GetInstance()->Clear();
	PtrDelete(pause);
	PtrDelete(result);
	PtrDelete(inGameTimer);
	PtrDelete(cansBar);
	ParticleManager::GetInstance()->ClearDeadEffect();
}

void Play::Initialize()
{
	next = Title;

	Object3D::SetCamera(camera.get());
	Object3D::SetLightGroup(lightGroup.get());

	objectManager->Reset();


	Player* player = new Player(Vector3(0, -5, -10));
	objectManager->Add(player);
	
	camera->Initialize();

	isEnd = false;
	isAllEnd = false;

	pause->Initialize();
	result->Initialize();
	inGameTimer->Initialize();
	cansBar->Initialize(100);//�ʂ̏�������n��

	//nowPlayingBGMName = "BGM_Play";
	//Audio::StopBGM(nowPlayingBGMName);
	//Audio::PlayBGM(nowPlayingBGMName, 0.1f * Audio::volume_bgm);


	Input::Update();
	Update();

	ParticleManager::GetInstance()->ClearDeadEffect();
}

void Play::Update()
{
	result->Update();
	if (result->GetIsToNextScene())
	{
		Audio::StopBGM(nowPlayingBGMName);
		Audio::AllStopSE();
		next = Title;
		ShutDown();
		return;
	}
	if (result->GetActivePause())
	{
		return;
	}
#ifdef _DEBUG
	//���U���g�J�n
	if (Input::TriggerKey(DIK_R))
	{
		result->IsActive(80, 50);//�ʂ̐��ƃW�����v�̉�
		return;
	}
#endif


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
	if (Input::TriggerKey(DIK_E))//�I������
	{
		Audio::StopBGM(nowPlayingBGMName);
		Audio::AllStopSE();
		ShutDown();
		return;
	}
#endif
	
	inGameTimer->Update();
	cansBar->Update(80);//�ʂ̌��ݐ���n��



	lightGroup->SetAmbientColor(XMFLOAT3(coloramb));
	lightGroup->SetDirLightDir(0, { lightDir[0],lightDir[1],lightDir[2],1 });
	lightGroup->Update();
	objectManager->Update();
	camera->Update();
	collisionManager->CheckAllCollisions();

	ParticleManager::GetInstance()->UpdateDeadEffect();
}

void Play::PreDraw()
{
	inGameTimer->Draw();
	cansBar->Draw();

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

	objectManager->PostDraw();
	if (!Object3D::GetDrawShadow())
	{
		DirectXLib::GetInstance()->DepthClear();
	}

	pause->Draw();
	result->Draw();
}


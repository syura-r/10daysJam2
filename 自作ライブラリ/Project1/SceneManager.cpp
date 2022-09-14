#include "SceneManager.h"

#include "Easing.h"
#include "Object3D.h"
#include "Sprite.h"
#include "TextureResource.h"


SceneManager* SceneManager::GetInstance()
{
	static SceneManager instance;
	//instance.Initialize();
	return &instance;
}

void SceneManager::Add(Scene::SCENE name, Scene * scene)
{
	if (scenes[name] != nullptr)
	{
		return;
	}
	scenes[name].reset(scene);
}

void SceneManager::Initialize()
{
	resource.reset(new TextureResource("migrateTex", { 1920,1080 }, DXGI_FORMAT_R8G8B8A8_UNORM, { 0,0,0,0 }, false));
	migrateTex = std::make_unique<Sprite>();
	migrateStart = false;
	//定数バッファの作成
	auto result = DirectXLib::GetInstance()->GetDevice()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDate) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&constBuff));
	assert(SUCCEEDED(result));
	migrateTime = 1.0f;
	migrateCounter = 0;

	transitionManager = std::make_unique<TransitionManager>();
	transitionManager->Initialize();
}

void SceneManager::Update()
{
	if (currentScene == nullptr)
	{
		return;
	}
	if (currentScene->GetIsEnd())
	{
		//変わる前のシーンのカメラの情報を受け取る
		if (!transitionManager->GetIsAction())
		{
			transitionManager->CameraChaser();
		}
		transitionManager->IsAction();

		if (transitionManager->GetChangeTime())
		{
			Change(currentScene->NextScene());
			currentScene->Update();

			//新しいシーンのカメラの情報を受け取る
			transitionManager->CameraChaser();
		}
	}
	else
	{
		currentScene->Update();
	}

	transitionManager->Update();
}

void SceneManager::Change(Scene::SCENE name)
{
	currentScene = scenes[name].get();
	currentScene->Initialize();
}

void SceneManager::PreDraw()
{
	if (currentScene == nullptr)
	{
		return;
	}


	//if (((!migrateStart && currentScene->GetIsEnd()) || migrateCounter == 60) && !Object3D::GetDrawShadow())
	//	resource->PreDraw();
	if (!migrateStart || migrateCounter == 60)
		currentScene->PreDraw();
}

void SceneManager::PostDraw()
{
	if (currentScene == nullptr)
	{
		return;
	}
	if (!migrateStart || migrateCounter == 60)
		currentScene->PostDraw();
	PipelineState::SetPipeline("Sprite");
	transitionManager->Draw();
}

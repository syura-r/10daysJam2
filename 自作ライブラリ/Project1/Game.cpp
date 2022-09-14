#include"Game.h"
#include"FPS.h"
#include"OBJLoader.h"
#include"Audio.h"
#include"ParticleEmitter.h"
#include"DebugText.h"
#include"FbxLoader.h"
#include"Object3D.h"
#include"Create3DObject.h"
#include"Texture.h"
#include"Scene.h"
#include"Title.h"
#include"Play.h"
#include"Alpha.h"
#include"Ending.h"
#include"PipelineState.h"
#include"FBXManager.h"
#include"DrawMode.h"
#include "ComputeShade.h"
#include "ComputeWrapper.h"
#include "GameSettingParam.h"
#include "LevelEditor.h"
#include"window.h"
#include"DirectXLib.h"
#include "LightCamera.h"
#include"SceneManager.h"
#include "Sprite.h"
#include "TextureResource.h"

DrawMode::MODE DrawMode::mode = DrawMode::None;
bool DrawMode::drawImGui = true;
TextureResource* TextureResource::mainResource = nullptr;

int SettingParam::padSensitive = 3;
int SettingParam::reverseX = 1;
int SettingParam::reverseY = 1;

int SettingParam::jumpButton = XINPUT_GAMEPAD_A;
int SettingParam::airSlideButton = XINPUT_GAMEPAD_B;
int SettingParam::runButton = XINPUT_GAMEPAD_LEFT_SHOULDER;
int SettingParam::resetButton = XINPUT_GAMEPAD_RIGHT_SHOULDER;
bool SettingParam::viewCollision = false;

bool SettingParam::onSSAO = false;

Game::Game()
{
	win = std::make_unique<Window>(1920,1080);
	directX = DirectXLib::GetInstance();
	computeWrapper = ComputeWrapper::GetInstance();
}

Game * Game::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new Game();
	}
	return instance;
}

Game::~Game()
{
}

void Game::RoadAsset()
{
	switch (loadAssetLevel)
	{
	case 0:
		//テクスチャの初期化
		Texture::Initialize();
		//テクスチャの読み込み
		Texture::LoadTexture("LoadDot", "LoadDot.png");
		Texture::LoadTexture("LoadPicture", "LoadPicture.png");
		break;
	case 1:
		Texture::LoadTexture("Debug", "ASCII.png");
		Texture::LoadTexture("particle", "particle1.png");
		Texture::LoadTexture("shock", "shock.png");
		Texture::LoadTexture("dust", "dust.png");
		Texture::LoadTexture("white1x1", "white1x1.png");
		Texture::LoadTexture("number", "number_white.png");
		Texture::LoadTexture("wind", "wind.png");

		Texture::LoadTexture("stopwatch", "timer/stopwatch.png");

		Texture::LoadTexture("frame", "stockCansBar/frame.png");
		Texture::LoadTexture("can", "stockCansBar/can.png");

		Texture::LoadTexture("select", "pause/select.png");
		Texture::LoadTexture("circle", "pause/circle.png");
		Texture::LoadTexture("toGame", "pause/toGame.png");
		Texture::LoadTexture("restart", "pause/restart.png");
		Texture::LoadTexture("toTitle", "pause/toTitle.png");
		Texture::LoadTexture("bgm", "pause/bgm.png");
		Texture::LoadTexture("se", "pause/se.png");

		Texture::LoadTexture("start", "title/start.png");
		Texture::LoadTexture("quit", "title/quit.png");

		Texture::LoadTexture("clear", "result/clear.png");
		Texture::LoadTexture("over", "result/over.png");
		Texture::LoadTexture("life", "result/life.png");
		Texture::LoadTexture("time", "result/time.png");

		Texture::LoadTexture("button_a", "button_a.png");

		Texture::LoadTexture("AuxiliaryLines", "AuxiliaryLines.png");

		
		break; 
	case 2:
		//Objモデルの読み込み
		OBJLoader::LoadModelFile("box", "box.obj", false);
		OBJLoader::LoadModelFile("sphere", "sphere.obj", true);
		OBJLoader::LoadModelFile("obBox", "bottomOriginBox.obj", false);
		OBJLoader::LoadModelFile("plate", "plate.obj", false);

		OBJLoader::LoadModelFile("can_title", "can_title.obj", false);
		OBJLoader::LoadModelFile("cover_title", "cover_title.obj", false);
		break;
	case 3:
		//Objモデルの読み込み
		break;
	case 4:
		//FBXファイルの読み込み
		FBXManager::LoadModelFile("player", "VendingMachine", false);
		FBXManager::LoadModelFile("badGuy", "Tinpira", false);
		FBXManager::LoadModelFile("crow", "crow", false);
		FBXManager::LoadModelFile("Veranda", "Veranda", false);
		FBXManager::LoadModelFile("Apartment", "Apartment", false);
		FBXManager::LoadModelFile("can", "can", false);
		FBXManager::LoadModelFile("Chara", "Chara", false);
		FBXManager::LoadModelFile("BossHead", "Tinpira_Head", false);
		FBXManager::LoadModelFile("BossBody", "Tinpira_Body", false);
		FBXManager::LoadModelFile("BossHair", "Tinpira_Hair", false);
		FBXManager::LoadModelFile("Boss", "Tinpira", false);
		FBXManager::LoadModelFile("BossNoHair", "Tinpira_B", false);


		FBXManager::LoadModelFile("LockOn", "LockOn", false);


		

		//WAVファイルの読み込み
		Audio::LoadFile("title", "BGM/title.wav");
		Audio::LoadFile("play", "BGM/play.wav");
		Audio::LoadFile("result", "BGM/result.wav");

		Audio::LoadFile("select", "SE/select.wav");
		Audio::LoadFile("decision", "SE/decision.wav");
		Audio::LoadFile("can_title", "SE/can_title.wav");
		Audio::LoadFile("jump", "SE/jump.wav");
		Audio::LoadFile("can", "SE/can1.wav");
		Audio::LoadFile("crow", "SE/crow.wav");
		Audio::LoadFile("trample", "SE/trample.wav");
		Audio::LoadFile("damage", "SE/playerdamage.wav");

		break;
	case 5:
		//FBXアニメーションの登録
		FBXManager::AddAnimationList("player", "stand", 0, 0);
		FBXManager::AddAnimationList("player", "jump",1,30);
		FBXManager::AddAnimationList("player", "onGraund", 31, 60);
		FBXManager::AddAnimationList("crow", "stop", 0, 0);
		FBXManager::AddAnimationList("crow", "move", 1, 60);
		FBXManager::AddAnimationList("badGuy", "stand", 0, 0);
		FBXManager::AddAnimationList("badGuy", "walk", 1, 120);
		FBXManager::AddAnimationList("badGuy", "attack", 121, 180);
		FBXManager::AddAnimationList("Boss", "stand", 0, 0);
		FBXManager::AddAnimationList("Boss", "walk", 1, 120);
		FBXManager::AddAnimationList("Boss", "attack",121, 174);
		FBXManager::AddAnimationList("Boss", "attackReady", 121, 159);

		//FBXManager::AddAnimationList("Boss", "attack", 165, 174);
		FBXManager::AddAnimationList("Boss", "attackEnd", 174, 158);

		FBXManager::AddAnimationList("BossNoHair", "stand", 0, 0);
		FBXManager::AddAnimationList("BossNoHair", "walk", 1, 120);
		FBXManager::AddAnimationList("BossNoHair", "attack", 174, 180);
		FBXManager::AddAnimationList("BossNoHair", "attackEnd", 180, 174);

		FBXManager::AddAnimationList("Chara", "1", 0, 0);
		FBXManager::AddAnimationList("Chara", "2", 3, 3);
		FBXManager::AddAnimationList("Chara", "3", 61, 61);

		break;
	default:
		loadAssetFinish = true;
		break;
	}
	loadAssetLevel++;
}

void Game::CreatePipeline()
{
	switch (createPipelineLevel)
	{
	case 0:
		PipelineState::CreatePipeline("Sprite", SPRITE);
		//PipelineState::CreatePipeline("DepthSprite", DepthSPRITE);

		break;
	case 1:
		PipelineState::CreatePipeline("FBX", FBX);
		PipelineState::CreatePipeline("BasicObj", BasicObj);

		break;
	case 2:
		PipelineState::CreatePipeline("Particle", PARTICLE, ADD);
		//PipelineState::CreatePipeline("FBXPolygonBreak", FBXPolygonBreak);
		break;
	case 3:
		PipelineState::CreatePipeline("NoShade", NoShade);

		PipelineState::CreatePipeline("UVScrolling", UVScrolling);
		PipelineState::CreatePipeline("AreaEffect", AREAEFFECT,ADD);


		break;
	case 4:
		PipelineState::CreatePipeline("CollisionBox", ViewCollisionBox);
		PipelineState::CreatePipeline("CollisionSphere", ViewCollisionSphere);
		break;
	case 5:
		break;
	case 6:
		//PipelineState::CreatePipeline("FBXShadowMap", FBXShadowMap);
		break;
	case 7:
		//PipelineState::CreatePipeline("ShadowMap", NormalShadowMap);
		break;
	case 8:
		//PipelineState::CreatePipeline("DrawShadowOBJ", DrawShadowOBJ);
		break;
	case 9:
		break;
	case 10:
		PipelineState::CreatePipeline("NoAlphaToCoverageSprite", NoAlphaToCoverageSprite, ADD);
		break;
	case 11:
		PipelineState::CreatePipeline("NoAlphaToCoverageSprite", NoAlphaToCoverageSprite);
		PipelineState::CreatePipeline("Migrate", Migrate);

		break;
		//case 12:
		//	break;
	default:
		createPipelineFinish = true;

		break;
	}

	createPipelineLevel++;
}

void Game::LoadFinish()
{
	//DirectInputオブジェクトの生成
	Input::Initialize(win->GetHwnd());

	CollisionManager::GetInstance()->Initialize(Vector3{ -10.0f,-15.0f,-30.0f }, Vector3{ 300.0f,10.0f,30.0f });
	

	lightCamera = std::make_unique<LightCamera>();
	lightCamera->SetDistance(100);
	lightCamera->SetLightDir({ dir[0],dir[1],dir[2] });
	Object3D::SetLightCamera(lightCamera.get());
	sceneManeger = SceneManager::GetInstance();
	sceneManeger->Initialize();
	sceneManeger->Add(Scene::SCENE::Title, new Title());
	sceneManeger->Add(Scene::SCENE::Play, new Play());
	//sceneManeger->Add(Scene::SCENE::Ending, new Ending());

	sceneManeger->Change(Scene::SCENE::Title);

	//shadowMap.reset(new TextureResource("shadowMap",{1920,1080}, DXGI_FORMAT_R32_FLOAT,{0,0,0,0}));

	nowLoading = false;
}

void Game::DrawLoadTex()
{
	loadTex->DrawSprite("LoadPicture", { 0,0 }, 0, { 1,1 }, { 1,1,1,1 }, { 0,0 });
	directX->DepthClear();
	loadDot->SpriteSetTextureRect("LoadDot", 0, 0, 42.0f * (createPipelineLevel % 8), 25);
	loadDot->DrawSprite("LoadDot", { 1560,1010 }, 0, { 1,1 }, { 1,1,1,1 }, { 0,0 });
}

void Game::Initialize()
{
	win->CreateWidow(Window::WINDOW);

	// DirectX 初期化処理 ここから
	directX->Initialize(win.get());
	computeWrapper->Initialize();

	// DirectX 初期化処理 ここまで
	FPS::SetWindow(win.get());
	FbxLoader::GetInstance()->Initialize();
	Sprite::StaticInitialize(win.get());
	Audio::Initialize();
	//ShowCursor(false);
	nowLoading = true;


	ComputeShade::StaticInitialize();
	
	ParticleEmitter::Initialize();
	FPS::Initialize();
	
#ifdef _DEBUG
	DebugText::Initialize();
#endif // _DEBUG

	loadTex = std::make_unique<Sprite>();
	loadDot = std::make_unique<Sprite>();

}

void Game::Run()
{
	MSG msg{}; // メッセージ
	while (true)
	{

		FPS::StartMeasure();
		// メッセージがある?
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg); // キー入力メッセージの処理
			DispatchMessage(&msg); // プロシージャにメッセージを送る
		}

		// 終了メッセージが来たらループを抜ける
		if (msg.message == WM_QUIT) {
			break;
		}

		if (nowLoading)
		{
			//アセットロード
			RoadAsset();
			//パイプラインの生成
			CreatePipeline();
			if (createPipelineFinish && loadAssetFinish)
				LoadFinish();
			directX->BeginDraw();
			directX->ImguiDraw();
			DrawLoadTex();

		}

		else if (!nowLoading)
		{
			if (sceneManeger->GetIsAllEnd())
			{
				break;
			}

			Input::Update();
			Alpha::Update();
#ifdef _DEBUG
			if (Input::TriggerKey(DIK_3))
			{
				DrawMode::SetDrawImGui(1 - DrawMode::GetDrawImGui());
			}
			if (Input::TriggerKey(DIK_4))
			{
				SettingParam::viewCollision = 1 - SettingParam::viewCollision;
			}

#endif // DEBUG

			//lightCamera->SetDistance(distance);
			//lightCamera->SetLightDir({ dir[0],dir[1],dir[2] });
			lightCamera->Update();
			Object3D::ClucLightViewProjection();
			sceneManeger->Update();
			ParticleManager::GetInstance()->Update();
			directX->ComputeBegin();
			//2.画面クリアコマンドここまで
			//Object3D::SetDrawShadow(true);
			//shadowMap->PreDraw();
			//directX->ImguiDraw();
			//sceneManeger->PreDraw();
			////directX->DepthClear();
			//shadowMap->PostDraw(false);
			Object3D::SetDrawShadow(false);

			//3.描画コマンドここから
			directX->BeginDraw();
			directX->ImguiDraw();

			sceneManeger->PreDraw();
			CollisionManager::GetInstance()->DrawCollider();
			directX->DepthClear();
			//背面描画ここまで
#ifdef _DEBUG
			DebugText::Draw();
#endif // _DEBUG
			//directX->DepthClear();

			sceneManeger->PostDraw();
			ParticleManager::GetInstance()->Draw();
		}
		directX->EndDraw();

		//ComputeWrapper::GetInstance()->MoveToNextFrame();
		LevelEditor::GetInstance()->Delete();
		FPS::FPSFixed();

	}

}

void Game::End()
{
	while (1)
	{
		if (ShowCursor(true) >= 0)
			break;
	}
	directX->End();
	computeWrapper->End();
	ParticleManager::GetInstance()->End();
#ifdef _DEBUG
	DebugText::End();
#endif // _DEBUG
	FbxLoader::GetInstance()->Finalize();
	FBXManager::DeleteModels();
	Audio::End();
	//デリートはここまでに終わらせる
	//ComputeWrapper::GetInstance()->End();
	win->End();
}

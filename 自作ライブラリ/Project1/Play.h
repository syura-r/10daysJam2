#pragma once
#include "Scene.h"
#include "LightGroup.h"
#include "CollisionManager.h"
#include "ComputeShade.h"
#include "DebugCamera.h"
#include "InGameCamera.h"
#include "Grass.h"
#include "NumberSprite.h"
#include "ObjectManager.h"
#include "Player.h"
#include "Sprite.h"
#include "Pause.h"
#include "Result.h"
#include "InGameTimer.h"
#include "StockCansBar.h"

class Play :public Scene
{
public:
	Play();
	~Play()override;
	void Initialize()override;
	void Update()override;
	void PreDraw()override;
	void PostDraw()override;
private:
	std::unique_ptr<LightGroup> lightGroup;
	XMFLOAT3 pos = { 1,0,0 };
	float coloramb[3] = { 1,1,1 };
	float color0[3] = { 1,1,1 };
	float color1[3] = { 1,1,1 };
	float color2[3] = { 1,1,1 };
	float lightDir[3] = { 0.0f,-1.00f,0.2f };
	bool targetSphere = false;
	CollisionManager* collisionManager = nullptr;
	ObjectManager* objectManager = nullptr;
	std::unique_ptr<InGameCamera> camera;
	
	Pause* pause = nullptr;
	Result* result = nullptr;
	InGameTimer* inGameTimer = nullptr;
	StockCansBar* cansBar = nullptr;

	std::string nowPlayingBGMName;
};


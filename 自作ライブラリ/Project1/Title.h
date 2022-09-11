#pragma once
#include "Scene.h"
#include "Object3D.h"
#include "DebugCamera.h"
#include "InGameCamera.h"
#include "LightGroup.h"
#include "Sprite.h"
#include "CanInTitle.h"

class Title :
	public Scene
{
public:
	Title();
	~Title()override;
	void Initialize() override;
	void Update()override;
	void PreDraw()override;
	void PostDraw()override;

private:
	std::unique_ptr<DebugCamera> camera;
	std::unique_ptr<LightGroup> lightGroup;

	//タイトルロゴの缶
	static const int cansNum = 2;
	CanInTitle* cans[cansNum];

	//
	Object3D* cover = nullptr;
	Vector3 position_cover = { 0,0,-3 };
	Vector3 scale_cover = { 17.5f,13.5f,13.5f };
	Vector3 rotation_cover = { 0,0,0 };
	XMFLOAT4 color_cover = { 1,1,1,1 };

	//項目選択
	void Select();
	int selectNumber = 0;
	const Vector2 scale_big = { 1.5f, 1.5f };
	const Vector2 scale_small = { 1.0f, 1.0f };
	const float position_x_top = 1620.0f;
	const float position_x_bottom = 1670.0f;
	int easingCount_scale = 0;
	const int easingCountLimit_scale = 10;

	//
	void AlphaChange_Base();
	Sprite* base = nullptr;
	Vector2 position_base = {};
	float alpha_base = 1.0f;
	bool isUP_alphaChange = false;

	//
	Sprite* start = nullptr;
	Vector2 position_start = {};
	Vector2 scale_start = {};

	//
	Sprite* quit = nullptr;
	Vector2 position_quit = {};
	Vector2 scale_quit = {};

	//決定ボタン表示
	Sprite* button = nullptr;
	const Vector2 position_button = { 1920.0f - 100.0f, 1080.0f - 70.0f };
	float alpha_button = 0.0f;

};

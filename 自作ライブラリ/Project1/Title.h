#pragma once
#include "Scene.h"
#include "Object3D.h"
#include "DebugCamera.h"
#include "InGameCamera.h"
#include"LightGroup.h"
#include"Sprite.h"

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
	std::unique_ptr<InGameCamera> camera;
	std::unique_ptr<LightGroup> lightGroup;

	Sprite* logo = nullptr;
	Vector2 position_logo = {};
	//ü‰ñ‚È‚Ç‚Ì’†S“_
	const Vector2 positionOrigin_logo = { 1920.0f / 2.0f, 400.0f };
	//ƒƒS‚Ìs“®’iŠK
	int actoinStep_logo = 0;

	//ƒƒS‚ÌƒtƒŒ[ƒ€ƒCƒ“
	void MoveStraight_Logo();
	//ƒƒS‚Ì‰ŠúˆÊ’u
	const Vector2 positionFirst_logo = { 1920.0f - 256.0f, -128.0f };
	int easingCount_position = 0;
	const int easingCountLimit_position = 60;

	//ƒƒS‚Ìü‰ñ
	void MoveCircle_Logo();
	//ü‰ñ‚ÌŒ»İ‚ÌŠp“x
	float angle_target = 0.0f;
	//ü‰ñ‹O“¹‚Ì‘È‰~‚ÌŒX‚«‹ï‡
	const float angleGap_target = -45.0f;
	//ü‰ñ‚Ì”¼Œa
	float radius_target = 0.0f;
	const float radiusMax_target = 25.0f;
	const float radiusMin_target = 15.0f;
	//ü‰ñ‚Ì’Ç]æ
	Vector2 position_target = {};

	//€–Ú‘I‘ğ
	void Select();
	int selectNumber = 0;
	const Vector2 scale_big = { 1.0f, 1.0f };
	const Vector2 scale_small = { 0.8f, 0.8f };
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
	const Vector2 position_start = { 1920.0f /2.0f, 700.0f };
	Vector2 scale_start = {};

	//
	Sprite* quit = nullptr;
	const Vector2 position_quit = { 1920.0f / 2.0f, 800.0f };
	Vector2 scale_quit = {};

	//Œˆ’èƒ{ƒ^ƒ“•\¦
	Sprite* button = nullptr;
	const Vector2 position_button = { 1920.0f - 100.0f, 1080.0f - 100.0f };
	float alpha_button = 0.0f;

};

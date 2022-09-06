#pragma once
#include "Scene.h"
#include"CreateFigure.h"
#include"Texture.h"
#include"Alpha.h"
#include "Object.h"
#include "TitleText.h"
#include "Sprite3D.h"
#include "Object3D.h"
#include "DebugCamera.h"
#include "InGameCamera.h"
#include"LightGroup.h"

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
	//�����_��
	float GetRandom(float arg_min, float arg_max);

	//�J�ڊ֘A
	//�J�����O�i
	bool ZoomIn();
	int easingTimer_zoom;

	std::unique_ptr<InGameCamera> camera;
	float cameraDistance;
	const float cameraDistance_init = 23.0f;
	std::unique_ptr<LightGroup> lightGroup;


	//�V�[���J�ڊJ�n
	bool isSceneChange = false;
};

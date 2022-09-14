#pragma once
#include "TransitionItem.h"

class TransitionManager
{
public:
	TransitionManager();
	~TransitionManager();

	void Initialize();
	void Update();
	void Draw();

	//シーン遷移開始
	void IsAction();
	bool GetIsAction();

	//シーンを変えるタイミングで真を返す
	bool GetChangeTime();

	//シーンが切り替わってカメラの位置が変わってもズレが無いようにする
	void CameraChaser();

private:
	bool isAction = false;
	//シーン遷移終了
	void IsActionEnd();

	//////////

	Vector3 parentPosition = {};
	Vector3 prevParentPosition = {};

	static const int itemsCount = 18;
	TransitionItem* items[itemsCount];
	//まばらに配置、初期化も
	bool ItemMapping();

	Vector3 cameraTarget = {};
};


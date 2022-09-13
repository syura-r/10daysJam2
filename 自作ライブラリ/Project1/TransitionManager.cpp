#include "TransitionManager.h"
#include "InGameCamera.h"

TransitionManager::TransitionManager()
{
	for (int i = 0; i < itemsCount; i++)
	{
		items[i] = new TransitionItem();
	}
}

TransitionManager::~TransitionManager()
{
	for (int i = 0; i < itemsCount; i++)
	{
		delete items[i];
	}
}

void TransitionManager::Initialize()
{
	isAction = false;

	//parentPosition = {};
	parentPosition.y += 10.0f;
	prevParentPosition = parentPosition;

	ItemMapping();
}

void TransitionManager::Update()
{
	if (!isAction)
		return;

	//シーン切り替え判定用
	prevParentPosition = parentPosition;

	//落下移動
	const float fallSpeed = 0.6f;
	parentPosition.y -= fallSpeed;
	for (int i = 0; i < itemsCount; i++)
	{
		items[i]->Update(parentPosition);
	}

	//終了
	IsActionEnd();
}

void TransitionManager::Draw()
{
	if (!isAction)
		return;

	PipelineState::SetPipeline("BasicObj");
	for (int i = 0; i < itemsCount; i++)
	{
		items[i]->Draw();
	}
}

void TransitionManager::IsAction()
{
	if (isAction)
		return;

	Initialize();
	isAction = true;
}

bool TransitionManager::GetIsAction()
{
	return isAction;
}

bool TransitionManager::GetChangeTime()
{
	bool isUnderTarget_prev = prevParentPosition.y < cameraTarget.y;
	bool isUnderTarget_on = parentPosition.y < cameraTarget.y;
	//cameraTarget.yを初めて越えた時に真を返す
	return isUnderTarget_prev != isUnderTarget_on;
}

void TransitionManager::CameraChaser()
{
	cameraTarget = Object3D::GetCamera()->GetTarget();
	parentPosition.x = cameraTarget.x;
	parentPosition.y = cameraTarget.y;
	parentPosition.z = cameraTarget.z;
}

void TransitionManager::IsActionEnd()
{
	const float limitBottom = cameraTarget.y - 30.0f;
	//limitBottom以下まで行ったらisActionを不にする
	if (parentPosition.y <= limitBottom)
	{
		isAction = false;
	}
}

bool TransitionManager::ItemMapping()
{
	const float positionXLenge = 3.0f;
	const float positionYLenge = 10.0f;
	const int rotationLenge = 360;

	const float positionsX[itemsCount / 2] = {
		-positionXLenge * 4,
		-positionXLenge * 3,
		-positionXLenge * 2,
		-positionXLenge * 1,
		positionXLenge * 0,//
		positionXLenge * 1,
		positionXLenge * 2,
		positionXLenge * 3,
		positionXLenge * 4,
	};

	int useNumber_positionsX = 0;
	for (int i = 0; i < itemsCount; i++)
	{
		float positionY = rand() % (int)positionYLenge;

		if (i <= itemsCount / 2)
		{
			useNumber_positionsX = i;
		}
		else
		{
			useNumber_positionsX = i - (itemsCount / 2);
			positionY += items[i - (itemsCount / 2)]->GetAddPosition().y + (positionYLenge / 2);
		}

		items[i]->Initialize(
			positionsX[useNumber_positionsX],
			positionY,
			rand() % rotationLenge);
	}

	return true;
}

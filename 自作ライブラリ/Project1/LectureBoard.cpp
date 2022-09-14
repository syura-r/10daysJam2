#include "LectureBoard.h"
#include "Input.h"
#include "Easing.h"
#include "InGameCamera.h"

LectureBoard::LectureBoard()
{
	sprite = new Sprite3D();
}

LectureBoard::~LectureBoard()
{
	delete sprite;
}

void LectureBoard::Initialize()
{
	position = Sprite3D::GetCamera()->GetTarget();

	alpha = 1.0f;
	easingCount = 0;

	inputClose = false;
}

void LectureBoard::Update()
{
	//表示終了へ
	if (Input::TriggerPadButton(XINPUT_GAMEPAD_A) || Input::TriggerKey(DIK_SPACE))
	{
		inputClose = true;
	}
	if (inputClose)
	{
		//徐々に薄く
		const int limit = 60 * 3;
		if (easingCount <= limit)
		{
			alpha = Easing::EaseInCubic(1.0f, 0.0f, limit, easingCount);
			easingCount++;
		}
	}


	//コントローラーが接続されているかで画像を変更
	if (Input::GetPadConnect())
	{
		texName = "lecture_pad";
	}
	else
	{
		texName = "lecture_key";
	}
}

void LectureBoard::Draw()
{
	sprite->DrawSprite(texName, position, 0.0f, { 0.1f, 0.1f }, { 1,1,1,alpha });
}

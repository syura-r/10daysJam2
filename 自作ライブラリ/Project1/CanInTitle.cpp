#include "CanInTitle.h"
#include "OBJLoader.h"
#include "Easing.h"
#include "Audio.h"

CanInTitle::CanInTitle(
	const float arg_positionX_first,
	const float arg_positionY_first, const float arg_positionY_end,
	const float arg_rotationZ_first, const float arg_rotationZ_end)
	:positionX_first(arg_positionX_first),
	positionY_first(arg_positionY_first), positionY_end(arg_positionY_end),
	rotationZ_first(arg_rotationZ_first), rotationZ_end(arg_rotationZ_end)
{
	object = Object3D::Create(OBJLoader::GetModel("can_title"), position, scale, rotation, color);
}

CanInTitle::~CanInTitle()
{
	delete object;
}

void CanInTitle::Initialize()
{
	position = { positionX_first,positionY_first,0.0f };
	rotation = { 135.0f,0.0f,rotationZ_first };

	isMoveEnd = false;
	easingCount_position = 0;
}

void CanInTitle::Update()
{
	FrameIn();

	object->Update();
}

void CanInTitle::Draw()
{
	object->Draw();
}

void CanInTitle::FrameIn()
{
	if (easingCount_position > easingCountLimit_position)
	{
		isMoveEnd = true;
		return;
	}

	if (easingCount_position == easingCountLimit_position / 5*2)
	{
		Audio::PlaySE("can_title", 0.1f * Audio::volume_se);
	}

	position.y = Easing::EaseOutBounce(positionY_first, positionY_end, easingCountLimit_position, easingCount_position);
	rotation.z = Easing::EaseInCubic(rotationZ_first, rotationZ_end, easingCountLimit_position, easingCount_position);

	easingCount_position++;
}

#include "TransitionItem.h"
#include "OBJLoader.h"

TransitionItem::TransitionItem()
{
	obj = Object3D::Create(OBJLoader::GetModel("can_title"), position, scale, rotation, color);
}

TransitionItem::~TransitionItem()
{
	delete obj;
}

void TransitionItem::Initialize(
	const float& arg_firstPositionX,
	const float& arg_firstPositionY,
	const float& arg_rotationZ)
{
	randAddPosition = { arg_firstPositionX,arg_firstPositionY,0.0f };
	position = randAddPosition;

	rotation = { 0.0f,-90.0f,arg_rotationZ };
}

void TransitionItem::Update(const Vector3& arg_parentPosition)
{
	position = randAddPosition + arg_parentPosition;
	obj->Update();
}

void TransitionItem::Draw()
{
	obj->Draw();
}

Vector3 TransitionItem::GetAddPosition()
{
	return randAddPosition;
}

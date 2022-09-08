#include "Crow.h"
#include"Player.h"
#include"FBXManager.h"
#include"Easing.h"
#include"CollisionAttribute.h"
Crow::Crow()
{
	StartPos = { player->GetPosition().x + 12 ,-1,0 };
	scale = { 1,0.5f,0.5f };
	color = { 1,0,0,1 };
	//�A�j���[�V�����p�Ƀ��f���̃|�C���^���i�[
	myModel = FBXManager::GetModel("badGuy");
	name = typeid(*this).name();
	//���f���̐���
	Create(myModel);
	BoxCollider* boxCollider = new BoxCollider();
	boxCollider->SetObject(this);
	boxCollider->SetOffset({ 0,0.25f,0,0 });
	boxCollider->SetScale(scale / 2);
	SetCollider(boxCollider);
	collider->SetAttribute(COLLISION_ATTR_ENEMYS);
	collider->SetMove(true);
	color = { 1,0,0,1 };
	name = typeid(*this).name();
	Initialize();

}

Crow::~Crow()
{
}

void Crow::Initialize()
{
	position = StartPos;
	prePos = position;
	moveCounter = 0;
}

void Crow::Update()
{
	moveCounter++;
	if (moveCounter <= 180 / mmoveVel)
	{
		position.x -= 0.1f * mmoveVel;
	}
	else if (moveCounter <= 210 / mmoveVel)
	{
		position.x -= 0.035f* mmoveVel;
		position.z -= 0.035f* mmoveVel;
	}
	else if (moveCounter <= 240 / mmoveVel)
	{
		position.x += 0.035f * mmoveVel;
		position.z -= 0.035f * mmoveVel;
		if (moveCounter == (int)(240 / mmoveVel))
		{
			velocity.z = -0.035f * mmoveVel;
		}
	}
	else if (moveCounter <= 305 / mmoveVel)
	{
		position.x += 0.1f * mmoveVel;
		position.z += velocity.z * mmoveVel;
		velocity.z += 0.035f / 65 * mmoveVel;
	}
	else if (moveCounter <= 370 / mmoveVel)
	{
		position.x += 0.1f * mmoveVel;
		position.z += velocity.z * mmoveVel;
		velocity.z += 0.035f / 65 * mmoveVel;
	}
	else if (moveCounter <= 400 / mmoveVel)
	{
		position.x += 0.035f * mmoveVel;
		position.z += 0.035f * mmoveVel;
		if (moveCounter == (int)(400 / mmoveVel))
		{
			targetPosY = player->GetPosition().y;
			velocity.z = -position.z / 60 * mmoveVel;
		}
	}
	else if (moveCounter <= 460 / mmoveVel)
	{
		position.y = Easing::EaseInOutSine(StartPos.y,targetPosY, 60 / mmoveVel,moveCounter - 400 / mmoveVel);
		position.z += velocity.z * mmoveVel;
	}
	else if (moveCounter <= 640 / mmoveVel)
	{
		position.x -= attackVel;
	}
	else 
	{
		velocity.y += 0.01f * mmoveVel;
		position.x -= 0.1f * mmoveVel;
		position.y += velocity.y * mmoveVel;
		if (position.y > 1)
			dead = true;
	}
	if (moveCounter > 180 / mmoveVel && moveCounter <= 240 / mmoveVel)
	{
		rotation.y = Easing::EaseInOutSine(0, -180, 60 / mmoveVel, moveCounter - 180 / mmoveVel);
		rotation.x = Easing::EaseInOutSine(0, -60, 60 / mmoveVel, moveCounter - 180 / mmoveVel);

	}
	else if (moveCounter > 370 / mmoveVel && moveCounter <= 400 / mmoveVel)
	{
		rotation.y = Easing::EaseInOutSine(-180, -270, 30 / mmoveVel, moveCounter - 370 / mmoveVel);
	}
	else if (moveCounter > 400 / mmoveVel && moveCounter <= 420 / mmoveVel)
	{
		rotation.z = Easing::EaseInOutSine(0, 90, 20 / mmoveVel, moveCounter - 400 / mmoveVel);
		rotation.y = Easing::EaseInOutSine( -270,-360, 20 / mmoveVel, moveCounter - 400 / mmoveVel);
		rotation.x = Easing::EaseInOutSine(-60, 0, 20 / mmoveVel, moveCounter - 400 / mmoveVel);
	}
	else if (moveCounter > 450 / mmoveVel && moveCounter <= 470 / mmoveVel)
	{
		rotation.z = Easing::EaseInOutSine(90, 0, 20 / mmoveVel, moveCounter - 450 / mmoveVel);
	}
	if (moveCounter <= 460 / mmoveVel)
		position.x += player->GetMoveDistance().x;
	Object::Update();
}

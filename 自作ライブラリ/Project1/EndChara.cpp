#include "EndChara.h"
#include"FBXManager.h"
#include"FBXModel.h"
#include"BoxCollider.h"
#include"CollisionAttribute.h"
#include"Input.h"
#include"CollisionManager.h"
#include "ParticleEmitter.h"
#include "Result.h"

EndChara::EndChara()
{
	rotation.y = -135;
	position = { 88, -7.5f, 0};
	//アニメーション用にモデルのポインタを格納
	myModel = FBXManager::GetModel("Chara");
	myModel->ChangeTexName("CharaTex2");
	name = typeid(*this).name();
	//モデルの生成
	Create(myModel);
	BoxCollider* boxCollider = new BoxCollider();
	boxCollider->SetObject(this);
	boxCollider->SetScale({ scale.x,scale.y * 2,scale.z  });
	SetCollider(boxCollider);
	collider->SetAttribute(COLLISION_ATTR_GOAL);
	//collider->SetMove(true);
	//color = { 1,0,0,1 };
	name = typeid(*this).name();
	Initialize();
	actionStart = false;
}

void EndChara::Update()
{
	BoxCollider* boxCollider = dynamic_cast<BoxCollider*>(collider);
	assert(boxCollider);
	if (CollisionManager::GetInstance()->CheckHitBox(*boxCollider, COLLISION_ATTR_BULLET, boxCollider))
	{
		actionStart = true;
		ParticleEmitter::CreateGetEffect(position);
		myModel->ChangeTexName("CharaTex3");

	}
	if (actionStart)
	{
		if (!myModel->PlayAnimation("4", false, 1, false))
		{
			Result::GetInstance()->IsActive(true);
		}
	}
	else
	{
		if (!myModel->PlayAnimation("2", false, 1, false)&&a)
		{
			a = false;
		}
		if (!a)
		{
			myModel->PlayAnimation("3", true, 1, false);
		}

	}
	Object::Update();
}

void EndChara::Draw()
{
	Object::CustomDraw(true, true);

}

void EndChara::DrawReady()
{
	pipelineName = "FBX";
}

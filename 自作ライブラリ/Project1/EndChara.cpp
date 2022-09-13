#include "EndChara.h"
#include"FBXManager.h"
#include"FBXModel.h"
#include"BoxCollider.h"
#include"CollisionAttribute.h"
#include"Input.h"

EndChara::EndChara()
{
	rotation.y = 180;
	position = { 0, -5, 0};
	//アニメーション用にモデルのポインタを格納
	myModel = FBXManager::GetModel("Chara");
	name = typeid(*this).name();
	//モデルの生成
	Create(myModel);
	//BoxCollider* boxCollider = new BoxCollider();
	//boxCollider->SetObject(this);
	////boxCollider->SetOffset({ 0,0.25f,0,0 });
	//boxCollider->SetScale({ scale.x * 2.5f,scale.y,scale.z * 2 });
	//SetCollider(boxCollider);
	//collider->SetAttribute(COLLISION_ATTR_ENEMYS);
	//collider->SetMove(true);
	//color = { 1,0,0,1 };
	name = typeid(*this).name();
	Initialize();
}

void EndChara::Update()
{
	if (Input::TriggerKey(DIK_0))
	{
		myModel->PlayAnimation("1",true,1,false);
	}
	if (Input::TriggerKey(DIK_9))
	{
		myModel->PlayAnimation("2", true, 1, false);
	}
	if (Input::TriggerKey(DIK_8))
	{
		myModel->PlayAnimation("3", true, 1, false);
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

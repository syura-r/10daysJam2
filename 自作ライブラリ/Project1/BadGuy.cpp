#include "BadGuy.h"
#include"FBXModel.h"
#include"FbxLoader.h"
#include"BoxCollider.h"
#include"CollisionAttribute.h"
#include "RaycastHit.h"
#include"CollisionManager.h"
#include"Player.h"
#include "PtrDelete.h"
#include "FBXManager.h"

BadGuy::BadGuy(const Vector3& arg_pos) 
{
	StartPos = arg_pos;
	//アニメーション用にモデルのポインタを格納
	myModel = FBXManager::GetModel("badGuy");

	name = typeid(*this).name();
	scale = 0.3f;
	//モデルの生成
	Create(myModel);
	BoxCollider* boxCollider = new BoxCollider();
	boxCollider->SetObject(this);
	boxCollider->SetOffset({ 0,0.60f,0,0 });
	boxCollider->SetScale(scale* Vector3{0.6f,2.25f,0.6f});
	SetCollider(boxCollider);
	collider->SetAttribute(COLLISION_ATTR_ENEMYS);
	collider->SetMove(true);
	name = typeid(*this).name();
	Initialize();
	Object::Update();

}

BadGuy::~BadGuy()
{
	//PtrDelete(myModel);
}

void BadGuy::Initialize()
{
	onGround = true;
	//scale = { 0.3f };
	position = StartPos;
	rotation.y = -90;
	prePos = position;
	direction = { 1,0,0 };
	velocity.x = -0.02f;
}

void BadGuy::Update()
{
	if (player == nullptr) return;
	if (Vector3(player->GetPosition() - position).Length() > 15)
		return;
	position.x += velocity.x;
		//落下処理
	if (!onGround)
	{
		//加速
		fallV.m128_f32[1] = max(fallV.m128_f32[1] + fallAcc, fallVYMin);
		//移動
		position.x += fallV.m128_f32[0];
		position.y += fallV.m128_f32[1];
		position.z += fallV.m128_f32[2];
	}
	if (position.y < -10)
		dead = true;
	CheckHit();
	myModel->PlayAnimation("walk", true, 2, true);

	Object::Update();
}

void BadGuy::CheckHit()
{
	//ボックスコライダーを取得
	BoxCollider* boxCollider = dynamic_cast<BoxCollider*>(collider);
	assert(boxCollider);

	//コライダー更新
	Object::Update();

	//ボックスの上端から球の下端までのレイキャスト用レイを準備
	Ray downRay;
	downRay.start = boxCollider->center;
	downRay.start.m128_f32[1] += boxCollider->GetScale().y;
	downRay.dir = { 0,-1,0,0 };
	RaycastHit downRayCastHit;

	//接地状態
	if (onGround)
	{
		//スムーズに坂を下る為の吸着距離
		const float absDistance = 0.2f;
		//接地を維持
		if (CollisionManager::GetInstance()->Raycast(downRay, boxCollider, COLLISION_ATTR_LANDSHAPE,
			&downRayCastHit, boxCollider->GetScale().y * 2.0f + absDistance))
		{
			onGround = true;
			position.y -= (downRayCastHit.distance - boxCollider->GetScale().y * 2.0f);
			//行列更新など
			Object::Update();
		}
		//地面がないので落下
		else {
			onGround = false;
			fallV = {};
		}
	}
	//落下状態
	else if (fallV.m128_f32[1] <= 0.0f)
	{
		if (CollisionManager::GetInstance()->Raycast(downRay, boxCollider, COLLISION_ATTR_LANDSHAPE,
			&downRayCastHit, boxCollider->GetScale().y * 2.0f))
		{
			//着地
			onGround = true;
			position.y -= (downRayCastHit.distance - boxCollider->GetScale().y * 2.0f);
			//行列更新など
			Object::Update();

			//if (fallV.m128_f32[1] <= fallVYMin + 0.3f)
			//	ParticleEmitter::CreateJumpDust(position);

		}
	}
	class PlayerQueryCallBack :public QueryCallback
	{
	public:
		PlayerQueryCallBack(Box* box) :box(box) {};

		//衝突時のコールバック関数
		bool OnQueryHit(const QueryHit& info)override
		{
			box->center += info.reject;
			move += info.reject;
			collider = info.collider;
			//}
			return true;
		}
		//クエリーに使用するボックス
		Box* box = nullptr;
		//衝突相手のコライダー
		BaseCollider* collider = nullptr;
		//排斥による移動量(累積値)
		DirectX::XMVECTOR move = {};
	};

	//クエリーコールバックの関数オブジェクト
	PlayerQueryCallBack callback(boxCollider);
	//callback.OnQueryHit(info);
	//地形との交差を全検索
	CollisionManager::GetInstance()->QueryBox(*boxCollider, &callback, COLLISION_ATTR_LANDSHAPE);
	Vector3 rejectVec = callback.move;
	rejectVec.Normalize();
	//交差による排斥文を動かす
	position.x += callback.move.m128_f32[0];
	position.y += callback.move.m128_f32[1];
	position.z += callback.move.m128_f32[2];

	if (callback.move.m128_f32[1] < 0 && fallV.m128_f32[1]>0)
		fallV.m128_f32[1] = 0;
	if (abs(callback.move.m128_f32[0]) > 0.01f)
	{
		velocity *= -1;
		rotation.y *= -1;
	}
}

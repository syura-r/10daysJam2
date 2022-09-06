#include "Player.h"

#include"OBJLoader.h"
#include"SphereCollider.h"
#include"Input.h"
#include"DebugText.h"
#include"CollisionManager.h"
#include"CollisionAttribute.h"
#include "DrawMode.h"
#include "Easing.h"
#include "FBXManager.h"
#include "GameSettingParam.h"
#include "ParticleEmitter.h"
#include "Audio.h"
#include "ParticleEmitter.h"
#include "AreaEffect.h"

Player::Player(const Vector3& arg_pos):StartPos(arg_pos)
{
	//アニメーション用にモデルのポインタを格納
	myModel = FBXManager::GetModel("player");
	name = typeid(*this).name();
	//モデルの生成
	Create(myModel);
	BoxCollider* boxCollider = new BoxCollider();
	boxCollider->SetObject(this);
	boxCollider->SetOffset({ 0,0.5f,0,0 });
	boxCollider->SetScale(scale / 2);
	SetCollider(boxCollider);
	collider->SetAttribute(COLLISION_ATTR_ALLIES);
	collider->SetMove(true);

	name = typeid(*this).name();
	Initialize();
}

Player::~Player()
{			
}

void Player::Initialize()
{	
	onGround = true;
	//scale = { 0.3f };
	position = StartPos;
	rotation = 0;
	prePos = position;
	direction = { 1,0,0 };
}

void Player::Update()
{
	//落下処理
	if (!onGround)
	{
		if (Input::DownKey(DIK_D))
		{
			position.x += 0.1f;
			direction = { 1,0,0 };
		}
		if (Input::DownKey(DIK_A))
		{
			position.x -= 0.1f;
			direction = { -1,0,0 };
		}

			//加速
			fallV.m128_f32[1] = max(fallV.m128_f32[1] + fallAcc, fallVYMin);
			//移動
			position.x += fallV.m128_f32[0];
			position.y += fallV.m128_f32[1];
			position.z += fallV.m128_f32[2];
	}
	//ジャンプ動作
	else if (!changeOnGroundScale && !jump)
	{
		jump = true;
		onGround = false;
		val += valVel;
		//ジャンプ時上向き初速
		jumpVYFist = 0.5f * val;
		//下向き加速
		fallAcc = -0.02f * val;

		fallV = { 0,jumpVYFist,0,0 };

		//ParticleEmitter::CreateShock(position);
	}
	JumpScaleCluc();
	CheckHit();
	Object::Update();
}
void Player::CheckHit()
{
	//ボックスコライダーを取得
	BoxCollider* boxCollider = dynamic_cast<BoxCollider*>(collider);
	assert(boxCollider);

	//コライダー更新
	Object::Update();
	Ray overDirectionRay;
	overDirectionRay.start = prePos.ConvertXMVECTOR() + boxCollider->GetOffset() + Vector3(0, boxCollider->GetScale().y - 0.01f, 0).ConvertXMVECTOR();
	overDirectionRay.dir = direction.ConvertXMVECTOR();
	RaycastHit overDirectionRayCastHit;
	if (CollisionManager::GetInstance()->Raycast(overDirectionRay, boxCollider, COLLISION_ATTR_LANDSHAPE,
		&overDirectionRayCastHit, 5))
	{
		float preToPosLength = Vector3(position - prePos).Length();
		if (preToPosLength > overDirectionRayCastHit.distance)
		{
			Vector3 rejectPos = overDirectionRayCastHit.inter - direction * boxCollider->GetScale().z;
			rejectPos.y = position.y;
			position = rejectPos;
		}
	}
	Ray underDirectionRay;
	underDirectionRay.start = prePos.ConvertXMVECTOR() + boxCollider->GetOffset() - Vector3(0, boxCollider->GetScale().y - 0.01f, 0).ConvertXMVECTOR();
	underDirectionRay.dir = direction.ConvertXMVECTOR();
	RaycastHit underDirectionRayCastHit;
	if (CollisionManager::GetInstance()->Raycast(underDirectionRay, boxCollider, COLLISION_ATTR_LANDSHAPE,
		&underDirectionRayCastHit, 5))
	{
		float preToPosLength = Vector3(position - prePos).Length();
		if (preToPosLength > underDirectionRayCastHit.distance)
		{
			Vector3 rejectPos = underDirectionRayCastHit.inter - direction * boxCollider->GetScale().z;
			rejectPos.y = position.y;
			position = rejectPos;
		}
	}

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
			jump = false;
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
			jump = false;
			position.y -= (downRayCastHit.distance - boxCollider->GetScale().y * 2.0f);
			//行列更新など
			Object::Update();
			changeOnGroundScale = true;

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
	Object::Update();

}

void Player::JumpScaleCluc()
{
	if (changeOnGroundScale)
	{
		changeScaleCounter++;
		const int EaingTime = 6;
		if (changeScaleCounter <= EaingTime)
		{
			scale.x = Easing::EaseOutExpo(1, 2, EaingTime, changeScaleCounter);
			scale.z = Easing::EaseOutExpo(1, 2, EaingTime, changeScaleCounter);
			scale.y = Easing::EaseOutExpo(1, 0.5f, EaingTime, changeScaleCounter);

		}
		else
		{
			scale.x = Easing::EaseOutExpo(2, 1, EaingTime, changeScaleCounter - EaingTime);
			scale.z = Easing::EaseOutExpo(2, 1, EaingTime, changeScaleCounter - EaingTime);
			scale.y = Easing::EaseOutExpo(0.5f, 1, EaingTime, changeScaleCounter - EaingTime);

		}
		if (changeScaleCounter >= EaingTime * 2)
		{
			changeOnGroundScale = false;
			changeJumpScale = true;
			changeScaleCounter = 0;
			return;
		}
	}
	if (!changeJumpScale)
		return;
	changeScaleCounter++;
	const int EaingTime = 6;
	if (changeScaleCounter <= EaingTime)
	{
		scale.x = Easing::EaseOutExpo(1, 0.7f, EaingTime, changeScaleCounter);
		scale.z = Easing::EaseOutExpo(1, 0.7f, EaingTime, changeScaleCounter);
		scale.y = Easing::EaseOutExpo(1, 1.5f, EaingTime, changeScaleCounter);

	}
	else
	{
		scale.x = Easing::EaseOutExpo(0.7f, 1, EaingTime, changeScaleCounter - EaingTime);
		scale.z = Easing::EaseOutExpo(0.7f, 1, EaingTime, changeScaleCounter - EaingTime);
		scale.y = Easing::EaseOutExpo(1.5f, 1, EaingTime, changeScaleCounter - EaingTime);

	}
	if (changeScaleCounter >= EaingTime * 2)
	{
		changeJumpScale = false;
		changeScaleCounter = 0;
		return;
	}

}

void Player::Draw()
{
	Object::CustomDraw(true,true);
}

void Player::DrawReady()
{
#ifdef _DEBUG
	//if (!Object3D::GetDrawShadow() && DrawMode::GetDrawImGui())
	//{
	//	XMMATRIX camMatWorld = XMMatrixInverse(nullptr, camera->GetMatView());
	//	Vector3 cameraDirectionZ = Vector3(camMatWorld.r[2].m128_f32[0], 0, camMatWorld.r[2].m128_f32[2]);
	//	cameraDirectionZ.Normalize();
	//	ImGui::Begin("PlayerStatus");
	//	ImGui::Text("CameraDirection : {%f, %f, %f }\n", cameraDirectionZ.x, cameraDirectionZ.y, cameraDirectionZ.z);
	//	ImGui::Text("Direction : {%f, %f, %f }\n", direction.x, direction.y, direction.z);
	//	ImGui::Text("Position : {%f, %f, %f }\n", position.x, position.y, position.z);
	//	ImGui::Text("Rot : {%f, %f, %f }\n", rotation.x, rotation.y, rotation.z);
	//	ImGui::Text("virtualityPlanePosition : {%f,%f,%f}\n", virtualityPlanePosition.x, virtualityPlanePosition.y, virtualityPlanePosition.z);
	//	ImGui::End();
	//}

#endif

	if (Object3D::GetDrawShadow())
	{
		pipelineName = "FBXShadowMap";
	}
	else
	{
		pipelineName = "FBX";
	}
}


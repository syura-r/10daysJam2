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
#include"HitStop.h"
#include"StockCansBar.h"
#include "PtrDelete.h"
#include "FallCan.h"
#include "ShotCan.h"
#include "Sprite3D.h"


Player::Player(const Vector3& arg_pos):StartPos(arg_pos)
{
	//アニメーション用にモデルのポインタを格納
	myModel = FBXManager::GetModel("player");
	name = typeid(*this).name();
	//モデルの生成
	Create(myModel);
	rotation = { 0,180 -45,0 };
	StartScale = 0.5f;
	scale = StartScale;
	BoxCollider* boxCollider = new BoxCollider();
	boxCollider->SetObject(this);
	//boxCollider->SetOffset({ 0,0.5f,0,0 });
	boxCollider->SetScale({ scale.x,scale.y*1.5f,scale.z*0.5f });
	SetCollider(boxCollider);
	collider->SetAttribute(COLLISION_ATTR_ALLIES);
	collider->SetMove(true);

	name = typeid(*this).name();
	cansBar = new StockCansBar();
	Initialize();
	auxiliaryLines = new Sprite3D();
}

Player::~Player()
{			
	PtrDelete(cansBar);
	PtrDelete(auxiliaryLines);
}

void Player::Initialize()
{	
	onGround = false;
	position = StartPos;
	prePos = position;
	direction = { 1,0,0 };
	jump = true;
	damage = false;
	knockBack = false;
	knockBackCounter = 0;
	damageCounter = 0;
	changeOnGroundScale = false;
	changeJumpScale = false;
	changeScaleCounter = 0;
	jumpCombo = false;
	comboCount = 0;
	goal = false;
	pushJumpVal = 1.0f;
	cansBar->Initialize(MaxJumpCount);//缶の初期数を渡す
	restJump = MaxJumpCount;
	rotVel = 0;
	fallV = {};
	fallAcc = -0.02f * val;
	nowRot = 0;
	stumble = false;
	stumbleCounter = 0;
	shotDir = {1,0,0};
	stumbleVelX = 0.3f;
	shotVel = 0;
	shotMoveFlag = false;
	cannotMoveRot = false;
}

void Player::Update()
{
	velocity = 0;
	prePos = position;
	downKeyFrame = false;
	//落下処理
	if (!onGround )
	{
		//rotation.x +=3.0f;
		//ノックバック時は移動できない
		if (!knockBack && !stumble)
		{
			if (Input::DownKey(DIK_D) || Input::CheckPadLStickRight())
			{
				velocity.x = 0.1f;
				direction = { 1,0,0 };
				downKeyFrame = true;
			}
			if (Input::DownKey(DIK_A) || Input::CheckPadLStickLeft())
			{
				velocity.x = -0.1f;
				direction = { -1,0,0 };
				downKeyFrame = true;
			}
			if ((Input::TriggerKey(DIK_SPACE) || Input::TriggerPadButton(XINPUT_GAMEPAD_RIGHT_SHOULDER) )&& !cannotMoveRot)
			{
				Shot();
			}
			if (jumpCombo)
				velocity *= 1.0f + 0.2 * jumpCombo;
			RotCluc();
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
		jumpCombo = false;
		comboCount = 0;
		a = false;
		jump = true;
		onGround = false;
		restJump--;
		val += valVel;
		//ジャンプ時上向き初速
		jumpVYFist = 0.5f * val * pushJumpVal;
		//下向き加速
		fallAcc = -0.02f * val;

		fallV = { 0,jumpVYFist,0,0 };
		pushJumpVal = 1.0f;

		//rotVel = -360.0f / abs(jumpVYFist / fallAcc * 2.0f);

		ParticleEmitter::CreateShock(position + Vector3{ 0, -0.5f, 0 });
		ObjectManager::GetInstance()->Add(new FallCan(position - Vector3{ 0,-0.2f,0.5f }));
	}
	//if(jump&& fallV.m128_f32[1] > 0)
	//	myModel->PlayAnimation("jump", false, 1, false);
	//if (jump && fallV.m128_f32[1] < 0)
	//	myModel->PlayAnimation("onGraund", false, 1, false);

	//else
	//{
	//	myModel->PlayAnimation("stand", true, 1, true);
	//}
	position += velocity;
	ShotMove();
	JumpScaleCluc();
	KnockBack();
	StumbleCluc();
	CheckHit();
	Damage();
	Object::Update();
	cansBar->Update(restJump);//缶の現在数を渡す
	Object::WorldUpdate(Vector3{ 0,0,rotVel });

	//object->WorldUpdate(Vector3{ 0,0,-rotVel }, NONE);
	nowRot += rotVel;

	if (nowRot < -360)
		nowRot += 360;
	else if (nowRot > 0)
		nowRot -= 360;
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
		else if(!a && stumble) {
			onGround = false;
			fallV.m128_f32[1] = 0;
			fallAcc = -0.0001f;
		}
	}
	//落下状態
	else if (fallV.m128_f32[1] <= 0.0f)
	{
		if (CollisionManager::GetInstance()->Raycast(downRay, boxCollider, COLLISION_ATTR_LANDSHAPE,
			&downRayCastHit, boxCollider->GetScale().y * 2.0f))
		{
			//着地
			Audio::PlaySE("jump", 0.1f * Audio::volume_se);
			onGround = true;
			jump = false;
			position.y -= (downRayCastHit.distance - boxCollider->GetScale().y * 2.0f);
			//行列更新など
			Object::Update();
			changeOnGroundScale = true;
				ParticleEmitter::CreateJumpDust(position + Vector3{0, -0.5f, 0});

			if (!stumble)
			{
				int gapRad = (int)nowRot % 360;
				if (gapRad < -340 || gapRad>-20 || (gapRad < -160 && gapRad>-200))
				{
					if (gapRad < -340)
					{
						rotVel = -360 - nowRot;
					}
					else if (gapRad > -20)
					{
						rotVel = -nowRot;
					}
					else 
					{
						rotVel = -180 - nowRot;
					}
					//着地
					onGround = true;
					jump = false;
					position.y -= (downRayCastHit.distance - boxCollider->GetScale().y * 2.0f);
					//行列更新など
					Object::Update();
					changeOnGroundScale = true;
					ParticleEmitter::CreateJumpDust(position + Vector3{ 0, -0.5f, 0 });
					myModel->PlayAnimation("stand", true, 1, false);
					if (!stumble && cannotMoveRot)
						cannotMoveRot = false;
				}
				else
				{
					stumble = true;
					rotVel = -nowRot / 20.0f;
					stumble = false;
					stumbleCounter = 0;
					//ジャンプ時上向き初速
					jumpVYFist = 0.05f;
					//下向き加速
					fallAcc = -jumpVYFist / 10;

					fallV = { 0,jumpVYFist,0,0 };

					cannotMoveRot = true;
				}
			}
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
	CollisionManager::GetInstance()->QueryBox(*boxCollider, &callback, COLLISION_ATTR_LANDSHAPE, (unsigned short)0xffffffff, collider);
	Vector3 rejectVec = callback.move;
	rejectVec.Normalize();
	//交差による排斥文を動かす
	position.x += callback.move.m128_f32[0];
	position.y += callback.move.m128_f32[1];
	position.z += callback.move.m128_f32[2];

	if (callback.move.m128_f32[1] < 0 && fallV.m128_f32[1]>0 && callback.move.m128_f32[0] == 0)
	{
		fallV.m128_f32[1] = 0;
	}
	else if (callback.move.m128_f32[1] > 0 && fallV.m128_f32[1] < 0 &&!downKeyFrame)
	{
		if (!stumble)
		{

			int gapRad = (int)nowRot % 360;
			if (gapRad < -340 || gapRad>-20 || (gapRad < -160 && gapRad>-200))
			{
				if (gapRad < -340)
				{
					rotVel = -360 - nowRot;
				}
				else if (gapRad > -20)
				{
					rotVel = -nowRot;
				}
				else
				{
					rotVel = -180 - nowRot;
				}
				onGround = true;
				jump = false;
				changeOnGroundScale = true;
				a = true;
				position.y += 0.05f;
				ParticleEmitter::CreateJumpDust(position + Vector3{ 0, -0.5f, 0 });
				if (!stumble && cannotMoveRot)
					cannotMoveRot = false;
			}
			else
			{
				stumble = true;
				rotVel = -nowRot/ 20.0f;
				stumble = false;
				stumbleCounter = 0;
				//ジャンプ時上向き初速
				jumpVYFist = 0.05f;
				//下向き加速
				fallAcc = -jumpVYFist / 10;

				fallV = { 0,jumpVYFist,0,0 };

				cannotMoveRot = true;
			}
		}
	}
	
	Object::Update();

	if (CollisionManager::GetInstance()->CheckHitBox(*boxCollider, COLLISION_ATTR_GOAL))
	{
		goal = true;
	}

	if (damage)
		return;
	//クエリーコールバックの関数オブジェクト
	PlayerQueryCallBack callback2(boxCollider);
	//敵との交差を全検索
	CollisionManager::GetInstance()->QueryBox(*boxCollider, &callback2, COLLISION_ATTR_ENEMYS, (unsigned short)0xffffffff,collider);
	Vector3 rejectVec2 = callback2.move;
	rejectVec2.Normalize();
	if (rejectVec2.Length() <= 0)
		return;
	//上から当たった場合
	if(rejectVec2.y>abs(rejectVec2.x))
	{
		HitStop::SetStopTime(5);		
		jump = true;
		onGround = false;
		jumpCombo = true;
		comboCount++;
		//ジャンプ時上向き初速
		jumpVYFist = 0.5f * val * (1.0f + 0.2f * comboCount);
		//下向き加速
		fallAcc = -0.02f * val;
		fallV = { 0,jumpVYFist,0,0 };
	}
	//横から当たった場合
	else
	{
		restJump -= 10;
		val += valVel * 10;
		damage = true;
		Audio::PlaySE("damage", 0.1f * Audio::volume_se);
		knockBack = true;
		if (rejectVec2.x > 0)
			knockBackVel = MaxNockBackVel;
		else
			knockBackVel = -MaxNockBackVel;
		jump = true;
		onGround = false;
		//ジャンプ時上向き初速
		jumpVYFist = 0.1f;
		//下向き加速
		fallAcc = -0.02f * val;
		fallV = { 0,jumpVYFist,0,0 };
		for (int i = 0; i < 10; i++)
		{
			ObjectManager::GetInstance()->Add(new FallCan(position));
		}
	}
}

void Player::JumpScaleCluc()
{
	if (changeOnGroundScale)
	{
		rotVel = 0;
		changeScaleCounter++;
		const int EaingTime = 12;
		if (changeScaleCounter <= EaingTime)
		{
			scale.x = Easing::EaseOutExpo(StartScale.x, StartScale.x * 2, EaingTime, changeScaleCounter);
			scale.z = Easing::EaseOutExpo(StartScale.z, StartScale.z * 2, EaingTime, changeScaleCounter);
			scale.y = Easing::EaseOutExpo(StartScale.y, StartScale.y * 0.5f, EaingTime, changeScaleCounter);

		}
		else
		{
			scale.x = Easing::EaseOutExpo(StartScale.x * 2, StartScale.x, EaingTime, changeScaleCounter - EaingTime);
			scale.z = Easing::EaseOutExpo(StartScale.z * 2, StartScale.z, EaingTime, changeScaleCounter - EaingTime);
			scale.y = Easing::EaseOutExpo(StartScale.y * 0.5f, StartScale.y, EaingTime, changeScaleCounter - EaingTime);

		}
		if (changeScaleCounter >= EaingTime * 2)
		{
			changeOnGroundScale = false;
			changeJumpScale = true;
			changeScaleCounter = 0;
			return;
		}
		if (Input::DownKey(DIK_SPACE))
		{
			pushJumpVal += 1.0f / (EaingTime * 2);
		}

	}
	if (!changeJumpScale)
		return;
	changeScaleCounter++;
	const int EaingTime = 10;
	if (changeScaleCounter <= EaingTime)
	{
		scale.x = Easing::EaseOutExpo(StartScale.x, StartScale.x * 0.7f, EaingTime, changeScaleCounter);
		scale.z = Easing::EaseOutExpo(StartScale.z, StartScale.z * 0.7f, EaingTime, changeScaleCounter);
		scale.y = Easing::EaseOutExpo(StartScale.y, StartScale.y * 1.5f, EaingTime, changeScaleCounter);

	}
	else
	{
		scale.x = Easing::EaseOutExpo(StartScale.x * 0.7f, StartScale.x, EaingTime, changeScaleCounter - EaingTime);
		scale.z = Easing::EaseOutExpo(StartScale.z * 0.7f, StartScale.z, EaingTime, changeScaleCounter - EaingTime);
		scale.y = Easing::EaseOutExpo(StartScale.y * 1.5f, StartScale.y, EaingTime, changeScaleCounter - EaingTime);

	}
	if (changeScaleCounter >= EaingTime * 2)
	{
		changeJumpScale = false;
		changeScaleCounter = 0;
		return;
	}

}

void Player::KnockBack()
{
	if (!knockBack)
		return;
	position.x += knockBackVel;
	const float knockBackAccel = MaxNockBackVel / 60.0f;
	knockBackVel += knockBackVel<0 ? knockBackAccel : -knockBackAccel;
	knockBackCounter++;
	if (knockBackCounter >= 60)
	{
		knockBack = false;
		knockBackCounter = 0;
	}
}

void Player::Damage()
{
	if (!damage)
		return;
	damageCounter++;

	if (damageCounter % 20 == 0)
	{
		color = { 1,1,1,0 };
	}
	else if (damageCounter % 10 == 0)
	{
		color = { 1,1,1,1 };
	}
	if (damageCounter >= MaxDamageTime)
	{
		damage = false;
		damageCounter = 0;
		color = { 1,1,1,1 };
	}
}

void Player::StumbleCluc()
{
	if (!stumble)return;
	if (stumbleCounter < 10)
	{
		//nowRot += rotVel;
	}
	else if (stumbleCounter < 40)
	{
		rotVel = 0;
		stumbleVelX += stumbleAccelX;
		position.x += stumbleVelX;
	}
	else if (stumbleCounter < 60)
	{
		int counter = stumbleCounter - 39;
		const int EaingTime = 10;
		if (counter <= EaingTime)
		{
			scale.z = Easing::EaseOutExpo(StartScale.z, StartScale.z * 2, EaingTime, counter);
			scale.y = Easing::EaseOutExpo(StartScale.y, StartScale.y * 2, EaingTime, counter);
			scale.x = Easing::EaseOutExpo(StartScale.x, StartScale.x * 0.5f, EaingTime, counter);

		}
		else
		{
			scale.z = Easing::EaseOutExpo(StartScale.z * 2, StartScale.z, EaingTime, counter - EaingTime);
			scale.y = Easing::EaseOutExpo(StartScale.y * 2, StartScale.y, EaingTime, counter - EaingTime);
			scale.x = Easing::EaseOutExpo(StartScale.x * 0.5f, StartScale.x, EaingTime, counter - EaingTime);

		}

	}
	if (stumbleCounter >= 60)
	{
		if (nowRot > -180)
		{
			rotVel = 90.0f / 20.0f;
		}
		else
		{
			rotVel = 270.0f / 20.0f;
		}
		stumble = false;
		stumbleCounter = 0;
		//ジャンプ時上向き初速
		jumpVYFist = 0.05f;
		//下向き加速
		fallAcc = -jumpVYFist/10;

		fallV = { 0,jumpVYFist,0,0 };

	}
	stumbleCounter++;

}

void Player::Shot()
{
	val += valVel;
	restJump--;
	//反動処理
	auto matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(nowRot));
	shotDir = XMVector3TransformNormal({ 1,0,0 }, matRot);
	shotDir.Normalize();
	const Vector3 shotReactionVel = -shotDir * shotReactionVal;
	shotVel.x += shotReactionVel.x;
	fallV.m128_f32[1] = shotReactionVel.y;
	fallAcc = -0.02f * MinVal * 1.3f;

	//発射処理
	ObjectManager::GetInstance()->Add(new ShotCan(position, shotDir));
	shotMoveFlag = true;
}

void Player::ShotMove()
{
	if (!shotMoveFlag) return;
	position.x += shotVel.x;
	if (shotVel.x > 0)
		shotVel.x -= 0.005f;
	else
		shotVel.x += 0.005f;
	if (shotVel.x < 0.01f && shotVel.x > -0.01f)
	{
		shotVel.x = 0;
		shotMoveFlag = false;
	}
}

void Player::RotCluc()
{
	if (cannotMoveRot)return;
	auto dir = Input::GetRStickDirection();
	rotVel = dir.x * 5.0f;

	if (Input::DownKey(DIK_RIGHT))
	{
		rotVel = 5.0f;
	}
	if (Input::DownKey(DIK_LEFT))
	{
		rotVel = -5.0f;
	}

	//if (Input::CheckPadRStickRight())
	//{
	//	rotVel = -4.0f;

	//}
	//if (Input::CheckPadRStickLeft())
	//{
	//	rotVel = 4.0f;
	//}
	//auto inputRot = Input::GetRStickDirection();
	//Vector2 inputDir = Vector2::Normalize({ inputRot.x,inputRot.y });
	//if (Vector2::Length(inputDir) <= 0)
	//{
	//	inputDir = { 1,0 };
	//}
	//auto matRot = XMMatrixIdentity();
	//matRot *= XMMatrixRotationZ(XMConvertToRadians(nowRot));
	//Vector3 nowDir = XMVector3TransformNormal({ 1,0,0 }, matRot);
	//nowDir.Normalize();
	//Vector2 nowDirVec2 = { nowDir.x,nowDir.y };
	//float alpha = XMConvertToDegrees(acosf(Vector2::Dot(inputDir, nowDirVec2)));
	//rotVel = alpha;
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

	auxiliaryLines->DrawSprite("AuxiliaryLines", position, nowRot, {0.3f,0.3f}, { 0,0.5f }, true);
		pipelineName = "FBX";
		cansBar->Draw();
}


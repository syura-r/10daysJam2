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
Player::Player(const Vector3& arg_pos):StartPos(arg_pos)
{
	//�A�j���[�V�����p�Ƀ��f���̃|�C���^���i�[
	myModel = FBXManager::GetModel("player");
	name = typeid(*this).name();
	//���f���̐���
	Create(myModel);
	rotation = { 0,180,0 };
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
	Initialize();
}

Player::~Player()
{			
}

void Player::Initialize()
{	
	onGround = true;
	position = StartPos;
	prePos = position;
	direction = { 1,0,0 };
	jump = false;
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
}

void Player::Update()
{
	velocity = 0;
	prePos = position;
	downKeyFrame = false;
	//��������
	if (!onGround)
	{
		//�m�b�N�o�b�N���͈ړ��ł��Ȃ�
		if (!knockBack)
		{
			if (Input::DownKey(DIK_D))
			{
				velocity.x = 0.1f;
				direction = { 1,0,0 };
				downKeyFrame = true;
			}
			if (Input::DownKey(DIK_A))
			{
				velocity.x = -0.1f;
				direction = { -1,0,0 };
				downKeyFrame = true;
			}
			if (jumpCombo)
				velocity *= 1.0f + 0.2 * jumpCombo;
		}
		//����
		fallV.m128_f32[1] = max(fallV.m128_f32[1] + fallAcc, fallVYMin);
		//�ړ�
		position.x += fallV.m128_f32[0];
		position.y += fallV.m128_f32[1];
		position.z += fallV.m128_f32[2];
	}
	//�W�����v����
	else if (!changeOnGroundScale && !jump)
	{
		jumpCombo = false;
		comboCount = 0;
		a = false;
		jump = true;
		onGround = false;
		val += valVel;
		//�W�����v�����������
		jumpVYFist = 0.5f * val * pushJumpVal;
		//����������
		fallAcc = -0.02f * val;

		fallV = { 0,jumpVYFist,0,0 };
		pushJumpVal = 1.0f;
		//ParticleEmitter::CreateShock(position);
	}
	position += velocity;
	JumpScaleCluc();
	KnockBack();
	CheckHit();
	Damage();
	Object::Update();
}
void Player::CheckHit()
{
	//�{�b�N�X�R���C�_�[���擾
	BoxCollider* boxCollider = dynamic_cast<BoxCollider*>(collider);
	assert(boxCollider);

	//�R���C�_�[�X�V
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

	//�R���C�_�[�X�V
	Object::Update();

	//�{�b�N�X�̏�[���狅�̉��[�܂ł̃��C�L���X�g�p���C������
	Ray downRay;
	downRay.start = boxCollider->center;
	downRay.start.m128_f32[1] += boxCollider->GetScale().y;
	downRay.dir = { 0,-1,0,0 };
	RaycastHit downRayCastHit;

	//�ڒn���
	if (onGround)
	{
		//�X���[�Y�ɍ������ׂ̋z������
		const float absDistance = 0.2f;
		//�ڒn���ێ�
		if (CollisionManager::GetInstance()->Raycast(downRay, boxCollider, COLLISION_ATTR_LANDSHAPE,
			&downRayCastHit, boxCollider->GetScale().y * 2.0f + absDistance))
		{
			onGround = true;
			jump = false;
			position.y -= (downRayCastHit.distance - boxCollider->GetScale().y * 2.0f);
			//�s��X�V�Ȃ�
			Object::Update();
		}
		//�n�ʂ��Ȃ��̂ŗ���
		else if(!a) {
			onGround = false;
			fallV = {};
		}
	}
	//�������
	else if (fallV.m128_f32[1] <= 0.0f)
	{
		if (CollisionManager::GetInstance()->Raycast(downRay, boxCollider, COLLISION_ATTR_LANDSHAPE,
			&downRayCastHit, boxCollider->GetScale().y * 2.0f))
		{
			//���n
			onGround = true;
			jump = false;
			position.y -= (downRayCastHit.distance - boxCollider->GetScale().y * 2.0f);
			//�s��X�V�Ȃ�
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

		//�Փˎ��̃R�[���o�b�N�֐�
		bool OnQueryHit(const QueryHit& info)override
		{
			box->center += info.reject;
			move += info.reject;
			collider = info.collider;
			//}
			return true;
		}
		//�N�G���[�Ɏg�p����{�b�N�X
		Box* box = nullptr;
		//�Փˑ���̃R���C�_�[
		BaseCollider* collider = nullptr;
		//�r�˂ɂ��ړ���(�ݐϒl)
		DirectX::XMVECTOR move = {};
	};

	//�N�G���[�R�[���o�b�N�̊֐��I�u�W�F�N�g
	PlayerQueryCallBack callback(boxCollider);
	//callback.OnQueryHit(info);
	//�n�`�Ƃ̌�����S����
	CollisionManager::GetInstance()->QueryBox(*boxCollider, &callback, COLLISION_ATTR_LANDSHAPE, (unsigned short)0xffffffff, collider);
	Vector3 rejectVec = callback.move;
	rejectVec.Normalize();
	//�����ɂ��r�˕��𓮂���
	position.x += callback.move.m128_f32[0];
	position.y += callback.move.m128_f32[1];
	position.z += callback.move.m128_f32[2];

	if (callback.move.m128_f32[1] < 0 && fallV.m128_f32[1]>0 && callback.move.m128_f32[0] == 0)
	{
		fallV.m128_f32[1] = 0;
	}
	else if (callback.move.m128_f32[1] > 0 && fallV.m128_f32[1] < 0 &&!downKeyFrame)
	{
		onGround = true;
		jump = false;
		changeOnGroundScale = true;
		a = true;

	}
	
	Object::Update();

	if (CollisionManager::GetInstance()->CheckHitBox(*boxCollider, COLLISION_ATTR_GOAL))
	{
		goal = true;
	}

	if (damage)
		return;
	//�N�G���[�R�[���o�b�N�̊֐��I�u�W�F�N�g
	PlayerQueryCallBack callback2(boxCollider);
	//�G�Ƃ̌�����S����
	CollisionManager::GetInstance()->QueryBox(*boxCollider, &callback2, COLLISION_ATTR_ENEMYS, (unsigned short)0xffffffff,collider);
	Vector3 rejectVec2 = callback2.move;
	rejectVec2.Normalize();
	if (rejectVec2.Length() <= 0)
		return;
	//�ォ�瓖�������ꍇ
	if(rejectVec2.y>abs(rejectVec2.x))
	{
		HitStop::SetStopTime(5);
		
		jump = true;
		onGround = false;
		jumpCombo = true;
		comboCount++;
		//�W�����v�����������
		jumpVYFist = 0.5f * val * (1.0f + 0.2f * comboCount);
		//����������
		fallAcc = -0.02f * val;

		fallV = { 0,jumpVYFist,0,0 };
	}
	//�����瓖�������ꍇ
	else
	{
		val += valVel * 10;
		damage = true;
		knockBack = true;
		if (rejectVec2.x > 0)
			knockBackVel = MaxNockBackVel;
		else
			knockBackVel = -MaxNockBackVel;
		jump = true;
		onGround = false;
		//�W�����v�����������
		jumpVYFist = 0.1f;
		//����������
		fallAcc = -0.02f * val;
		fallV = { 0,jumpVYFist,0,0 };
	}
}

void Player::JumpScaleCluc()
{
	if (changeOnGroundScale)
	{
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


#pragma once
#include"ParticleManager.h"
#include"Camera.h"
#include "ObjectManager.h"

class Line;
class ParticleEmitter
{
private:
    static ParticleManager* particleManager;
    static ObjectManager* p_objectManager;

public:
    static void Initialize();

    static void SetObjectManager(ObjectManager* arg_objManager);

    //パネルカット時のエフェクト(arg_pos:生成場所, arg_vec:進行方向)
    static void CutEffect(const Vector3& arg_pos, const Vector3& arg_vec);

    //衝突時のエフェクト(arg_pos:生成場所)
    static void ShockEffect(const Vector3& arg_pos, const Vector3& arg_color);

    //パネル回収時エフェクト
    static void PieceGetEffect(const Vector3& arg_position, const Vector3& arg_scale, const Vector3& arg_rotation, const DirectX::XMFLOAT4& arg_color, Object* arg_getActorObj);

    //フィーバーエフェクト
    static void FeverEffect(const Vector3& arg_position);

    static void DeadEffect(const Vector3& arg_position, const DirectX::XMFLOAT4& arg_color);

    static void CreateLaser(const Vector3& arg_position, const float& arg_rot);


    static void CreateShock(const Vector3& pos, const Vector3& arg_rotation = { XM_PI * 0.5f,0,0 });
    static void CreateBossAppearShock(const Vector3& pos);

    static void CreateGetEffect(const Vector3& pos);
    static void CreateMagicEffect(const Vector3& pos);

    static void CreateJumpDust(const Vector3& pos);

private:
    static float GetRandom(float arg_min, float arg_max);
};
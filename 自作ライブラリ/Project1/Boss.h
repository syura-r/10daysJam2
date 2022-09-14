#pragma once
#include "BaseEnemy.h"
class FBXModel;
class InGameCamera;
class HitPointBar;
class Sprite;
class Boss :
    public BaseEnemy
{
public:
    Boss();
    ~Boss();
    void Initialize()override;
    void Update()override;
    void Draw()override;
    void DrawReady()override;
    void OnCollision(const CollisionInfo& info)override;
    void StartApper();
    void StartMagic();
    void Break();
private:
    void Move();
    void Appear();
    void Attack();
    void ChangeColor();
    void CheckHit();
    void Damage();
    void Death();
    FBXModel* mainModel = nullptr;
    FBXModel* noMohikanModel = nullptr;
    FBXModel* mohikanModel = nullptr;
    FBXModel* headModel = nullptr;
    FBXModel* bodyModel = nullptr;
    const int MaxHP = 200;
    int hp = MaxHP;
    const int DamageVal = 10;
    bool damage = false;
    bool magic;
    bool earthquake;
    bool onGround;
    XMVECTOR fallV = {};
    int colorChangeCounter;
    int appearCounter;
    int attackCounter;
    int jumpCounter = 0;
    int damageCounter = 0;
    bool wallHit;
    bool rightRush;
    const float StartPosX = 10;
    XMFLOAT4 preColor;
    float fallAcc = -0.02f * 0.4f;
    //下向き加速度の最大値
    const float fallVYMin = -0.8f;
    bool breakModelDraw = false;
    Sprite* flashTex = nullptr;
    bool drawFlash;
    float flashAlpha;
    int flashCounter;

    //死亡アニメーションフラグ
    bool playBreakAnimation = false;
    //死亡アニメーションの速度
    const float breakSpeed = 0.01f;
    ComPtr<ID3D12Resource> constBuff; // 定数バッファ
    float destruction;
    float scaleFactor; //スケールの変化量
    float positionFactor;//ポジションの変化量
    float rotationFactor;//回転の変化量
    int  tessellation;//ポリゴン分割度
    bool  onEasing;//ポリゴン分割度
    struct ConstBuffData
    {
        float _Destruction; //分解度合い
        float _ScaleFactor; //スケールの変化量
        float _PositionFactor;//ポジションの変化量
        float _RotationFactor;//回転の変化量
        int _Tessellation;//ポリゴン分割度
        int _OnEasing;//イージングで分解するか
    };

    InGameCamera* camera = nullptr;
    enum class ActionState
    {
        appear,
        await,
        attack,
        move,
        death
    };
    ActionState actionState;

    enum class AppearState
    {
        masgic,
        scaleUp,
        jump,
        earthquake,
    };
    AppearState appearState;

    enum class AttackState
    {
        rush,
        beam,
        jump,
        boomerang,
    };
    AttackState attackState;

    Object* lockOnObj = nullptr;
    HitPointBar* hpBar = nullptr;
};


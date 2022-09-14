#pragma once
#include "BaseEnemy.h"
class FBXModel;
class InGameCamera;
class HitPointBar;
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
    void EndMagic();
private:
    void Move();
    void Appear();
    void Attack();
    void ChangeColor();
    void CheckHit();
    FBXModel* mainModel = nullptr;
    FBXModel* headModel = nullptr;
    FBXModel* bodyModel = nullptr;
    const int MaxHP = 300;
    int hp = MaxHP;
    bool magic;
    bool earthquake;
    bool onGround;
    XMVECTOR fallV = {};
    int colorChangeCounter;
    int appearCounter;
    int attackCounter;
    int jumpCounter = 0;
    bool wallHit;
    bool rightRush;

    XMFLOAT4 preColor;
    float fallAcc = -0.02f * 0.4f;
    //â∫å¸Ç´â¡ë¨ìxÇÃç≈ëÂíl
    const float fallVYMin = -0.8f;

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

    HitPointBar* hpBar = nullptr;
};


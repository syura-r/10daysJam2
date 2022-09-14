#pragma once
#include "Object.h"
class FBXModel;
class InGameCamera;
class Boss :
    public Object
{
public:
    Boss();
    ~Boss();
    void Initialize()override;
    void Update()override;
    void Draw()override;
    void DrawReady()override;
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
    XMFLOAT4 preColor;
    float fallAcc = -0.02f * 0.4f;
    //â∫å¸Ç´â¡ë¨ìxÇÃç≈ëÂíl
    const float fallVYMin = -0.5f;

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
};


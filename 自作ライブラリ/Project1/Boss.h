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
    //�����������x�̍ő�l
    const float fallVYMin = -0.8f;
    bool breakModelDraw = false;
    Sprite* flashTex = nullptr;
    bool drawFlash;
    float flashAlpha;
    int flashCounter;

    //���S�A�j���[�V�����t���O
    bool playBreakAnimation = false;
    //���S�A�j���[�V�����̑��x
    const float breakSpeed = 0.01f;
    ComPtr<ID3D12Resource> constBuff; // �萔�o�b�t�@
    float destruction;
    float scaleFactor; //�X�P�[���̕ω���
    float positionFactor;//�|�W�V�����̕ω���
    float rotationFactor;//��]�̕ω���
    int  tessellation;//�|���S�������x
    bool  onEasing;//�|���S�������x
    struct ConstBuffData
    {
        float _Destruction; //����x����
        float _ScaleFactor; //�X�P�[���̕ω���
        float _PositionFactor;//�|�W�V�����̕ω���
        float _RotationFactor;//��]�̕ω���
        int _Tessellation;//�|���S�������x
        int _OnEasing;//�C�[�W���O�ŕ������邩
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


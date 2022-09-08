#pragma once
#include "Object.h"
class Player;
class FBXModel;
class BaseEnemy :
    public Object
{
public:
    void OnCollision(const CollisionInfo& info)override;
    void Draw()override;
    void DrawReady()override;
    static void SetPlayerPtr(Player* arg_ptr)
    {
        player = arg_ptr;
    }
protected:
  static  Player* player;
  FBXModel* myModel = nullptr;
  Vector3 StartPos;
  Vector3 prePos;
};


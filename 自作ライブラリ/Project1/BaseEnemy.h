#pragma once
#include "Object.h"
class Player;
class BaseEnemy :
    public Object
{
public:
    BaseEnemy(Player* playerPtr);
protected:
    Player* player = nullptr;
};


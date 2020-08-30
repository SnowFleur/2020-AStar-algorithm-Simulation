#pragma once
#include"Controller.h"

class CFoodController:public Controller{
private:
public:
    void          Update(CGameObject& gameobject, CWorld& world)override;

};


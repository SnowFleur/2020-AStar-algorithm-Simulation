#pragma once
#include"Controller.h"

constexpr int WORKER_ANT_CREATED_DATE   = 10;
constexpr int MALE_ANT_CREATED_DATE     = 5;
constexpr int WORKER_LIFE               = 365;
constexpr int MALE_LIFE                 = 183;

class QUEEN_ACTION : public BASE_ACTION {
private:
public:
    enum {
        ACTION_CREATED_ANT = BASE_ACTION::ACTION_DIE + 1,
    };
};

class CQueenController: public Controller{
private:
    QUEEN_ACTION queenAction_;
public:
    CQueenController();
    void Update(CGameObject& gameobject, CWorld& world)override;
};


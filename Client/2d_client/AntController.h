#pragma once
#include"Controller.h"
#include"GameObject.h"

using Node = std::pair<int, Position>;
constexpr int INF = 500;

class ANT_ACTION :public BASE_ACTION {
private:
public:
    enum {
        ACTION_EAT = BASE_ACTION::ACTION_DIE + 10,
        ACTION_MOVE,
        ACTION_BREAK,
    };
};

struct compare {
public:
    compare() = delete;
    bool operator()(const Node& lhs, const Node& rhs) {
        return lhs.first > rhs.first;
    }
};

class CAntController : public Controller {
private:
    WORD lastEatDay_;
    ANT_ACTION antAction_;
public:
    CAntController();
    ~CAntController() = default;

    void          Update(CGameObject& gameobject, CWorld& world)override;
    CGameObject* IsArroundFood(CWorld& world, CGameObject& gameobject);
    void          CompareDirection(CWorld& world, CGameObject& gameobject);
    WORD          SearchFood(CWorld& world, const WORD x, const WORD y);
};


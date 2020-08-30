#include "Navigation.h"


CNavigation::CNavigation() :weight_{ MAX_WEIGHT}, isMove_{ true }{}

void CNavigation::SetMoveAble(bool isMove) {
    isMove_ = isMove;
}

bool CNavigation::GetMoveAble()const {
    return isMove_;
}

void CNavigation::SetWeight(int weight) {
    weight_ = weight;
}

int CNavigation::GetWeight()const {
    return weight_;
}

void CNavigation::ResetData() {
    isMove_ = true;
    weight_ = MAX_WEIGHT;
}
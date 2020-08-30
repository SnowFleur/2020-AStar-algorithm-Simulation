#include<utility>
#include"GameObject.h"

CGameObject::CGameObject(GAME_OBJECT_TYPE ObjectType, Position&& position, const WORD objectLife) :
    objectType_{ ObjectType }, position_{ std::move(position) }, obejectLife_{ objectLife },
    isUsed_{ true }, textureDirection_{ TEXTURE_DIRECTION::TEXTURE_UP }{}

CGameObject::CGameObject(GAME_OBJECT_TYPE ObjectType, const Position& position, const WORD objectLife) :
    objectType_{ ObjectType }, position_{ position }, obejectLife_{ objectLife }, 
    isUsed_{ true }, textureDirection_{ TEXTURE_DIRECTION::TEXTURE_UP }{}

Position&& CGameObject::GetPosition() {
    return std::move(position_);
}
void CGameObject::SetPosition(Position&& position){
    position_ = std::move(position);
}

void CGameObject::SetPosition(const Position& position) {
    position_ = position;
}

void CGameObject::SetPosition(const WORD x, const WORD y) {
    position_.x = x;
    position_.y = y;
}

GAME_OBJECT_TYPE CGameObject::GetGameObjectType()const {
    return objectType_;
}


WORD CGameObject::GetObjectLife()const {
    return obejectLife_;
}

bool CGameObject::GetIsUsed()const {
    return isUsed_;
}

void CGameObject::SetIsUsed(const bool used) {
    isUsed_ = used;
}
TEXTURE_DIRECTION CGameObject::GetTextureDirection()const {
    return textureDirection_;
}

void CGameObject::SetTextureDirection(TEXTURE_DIRECTION direction) {
    textureDirection_ = direction;
}

void CGameObject::DecreaseObjectLife() {
    if (obejectLife_ != 0) {
        --obejectLife_;
    }
}

void CGameObject::SetObjectLife(const WORD life) {
    obejectLife_ = life;
}
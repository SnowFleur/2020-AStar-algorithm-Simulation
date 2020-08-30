#include<utility>
#include"Controller.h"
#include"directX.h"
#include"Texture.h"
#include "Food.h"

CTexture* CFood::foodtexture_ = nullptr;
DWORD CFood::spawnTime_ = 0;

CFood::CFood(const Position& position, const WORD objectLife, GAME_OBJECT_TYPE objectType, Controller* controller) :
    CGameObject{ objectType, position,objectLife }, controller_{ controller }{}

CFood::CFood(Position&& position, const WORD objectLife, GAME_OBJECT_TYPE objectType, Controller* controller) :
    CGameObject{ objectType, std::move(position),objectLife }, controller_{ controller }{}

void CFood::SetGameObjectTexture(const wchar_t* fileName, const size_t width,
    const size_t height, const BYTE cycle) {
    if (foodtexture_ == nullptr) {
        foodtexture_ = new CTexture(fileName, width, height, cycle);
    }
}

void CFood::SetSpawnTime(DWORD time) {
    spawnTime_ = time;
}

DWORD CFood::GetSpawnTime() {
    return spawnTime_;
}

void CFood::Update(CWorld& world) {
    controller_->Update(*this, world);
}

void CFood::Draw() {
    wchar_t text[10]{};
    Position objectPostion = GetPosition();

    if (foodtexture_ != nullptr) {
        foodtexture_->Draw(objectPostion.x, objectPostion.y,GetTextureDirection());

        D3DXVECTOR3 pos = D3DXVECTOR3((objectPostion.x - g_left_x) * 65.0f + 8,
            (objectPostion.y - g_top_y) * 65.0f + 8, 0.0);

        wsprintf(text, L"size:%d", (BYTE)GetObjectLife());
        Draw_Text_D3D(text, static_cast<int>(pos.x - 5), static_cast<int>(pos.y - 15), D3DCOLOR_ARGB(255, 255, 0, 0));
    }
}
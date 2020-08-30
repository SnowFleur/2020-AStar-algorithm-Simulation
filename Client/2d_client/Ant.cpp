#include<utility>
#include "Ant.h"
#include"Controller.h"
#include"Texture.h"

#pragma region QueenAnt

CTexture* CQueenAnt::queenAntTexture_ = nullptr;

CQueenAnt::CQueenAnt(const Position& position, const WORD objectLife, GAME_OBJECT_TYPE objectType, Controller* controller) :
    CGameObject{ objectType, position,objectLife },
    controller_{ controller }, textureDirection_{ ANT_TEXTURE_DIRECTION::TEXTURE_UP }{}

CQueenAnt::CQueenAnt(Position&& position, const WORD objectLife, GAME_OBJECT_TYPE objectType, Controller* controller) :
    CGameObject{ objectType, std::move(position),objectLife },
    controller_{ controller }, textureDirection_{ ANT_TEXTURE_DIRECTION::TEXTURE_UP }{}

void CQueenAnt::SetGameObjectTexture(const wchar_t* fileName, const size_t width,
    const size_t height, const BYTE cycle) {

    if (queenAntTexture_ == nullptr) {
        queenAntTexture_ = new CTexture(fileName, width, height, cycle);
    }
}

void CQueenAnt::Update(CWorld& world) {
    controller_->Update(*this, world);
}

void CQueenAnt::Draw() {
    Position objectPostion = GetPosition();
    if (queenAntTexture_ != nullptr) {

        D3DXVECTOR3 pos = D3DXVECTOR3((objectPostion.x - g_left_x) * 65.0f + 8,
            (objectPostion.y - g_top_y) * 65.0f + 8, 0.0);

        wchar_t text[10]{};
        wsprintf(text, L"Life:%d", (int)GetObjectLife());
        Draw_Text_D3D(text, static_cast<int>(pos.x), static_cast<int>(pos.y), D3DCOLOR_ARGB(255, 255, 0, 0));

        queenAntTexture_->Draw(objectPostion.x, objectPostion.y, GetTextureDirection());
    }
}

#pragma endregion

#pragma region workerAnt
/*
WorkerAnt
*/

CTexture* CWorkerAnt::workerAntTexture_ = nullptr;

CWorkerAnt::CWorkerAnt(const Position& position, const WORD objectLife, GAME_OBJECT_TYPE objectType, Controller* controller) :
    CGameObject{ objectType, position,objectLife },
    controller_{ controller }, textureDirection_{ ANT_TEXTURE_DIRECTION::TEXTURE_UP }{}

CWorkerAnt::CWorkerAnt(Position&& position, const WORD objectLife, GAME_OBJECT_TYPE objectType, Controller* controller) :
    CGameObject{ objectType, std::move(position),objectLife },
    controller_{ controller }, textureDirection_{ ANT_TEXTURE_DIRECTION::TEXTURE_UP }{}

void CWorkerAnt::SetGameObjectTexture(const wchar_t* fileName, const size_t width,
    const size_t height, const BYTE cycle) {

    if (workerAntTexture_ == nullptr) {
        workerAntTexture_ = new CTexture(fileName, width, height, cycle);
    }
}

void CWorkerAnt::Update(CWorld& world) {
    controller_->Update(*this, world);
}

void CWorkerAnt::Draw() {
    Position objectPostion = GetPosition();
    if (workerAntTexture_ != nullptr) {

        D3DXVECTOR3 pos = D3DXVECTOR3((objectPostion.x - g_left_x) * 65.0f + 8,
            (objectPostion.y - g_top_y) * 65.0f + 8, 0.0);

        wchar_t text[10]{};
        wsprintf(text, L"Life:%d", (int)GetObjectLife());
        Draw_Text_D3D(text, static_cast<int>(pos.x), static_cast<int>(pos.y), D3DCOLOR_ARGB(255, 255, 0, 0));
        workerAntTexture_->Draw(objectPostion.x, objectPostion.y, GetTextureDirection());
    }
}


#pragma endregion

#pragma region MaleAnt
/*
MaleAnt
*/
CTexture* CMaleAnt::maleAntTexture_ = nullptr;

CMaleAnt::CMaleAnt(const Position& position, const WORD objectLife, GAME_OBJECT_TYPE objectType, Controller* controller) :
    CGameObject{ objectType, position,objectLife },
    controller_{ controller }, textureDirection_{ ANT_TEXTURE_DIRECTION::TEXTURE_UP }{}

CMaleAnt::CMaleAnt(Position&& position, const WORD objectLife, GAME_OBJECT_TYPE objectType, Controller* controller) :
    CGameObject{ objectType, std::move(position),objectLife },
    controller_{ controller }, textureDirection_{ ANT_TEXTURE_DIRECTION::TEXTURE_UP }{}

void CMaleAnt::SetGameObjectTexture(const wchar_t* fileName, const size_t width,
    const size_t height, const BYTE cycle) {

    if (maleAntTexture_ == nullptr) {
        maleAntTexture_ = new CTexture(fileName, width, height, cycle);
    }
}

void CMaleAnt::Update(CWorld& world) {
    controller_->Update(*this, world);
}

void CMaleAnt::Draw() {
    Position objectPostion = GetPosition();
    if (maleAntTexture_ != nullptr) {

        D3DXVECTOR3 pos = D3DXVECTOR3((objectPostion.x - g_left_x) * 65.0f + 8,
            (objectPostion.y - g_top_y) * 65.0f + 8, 0.0);

        wchar_t text[10]{};
        wsprintf(text, L"Life:%d", (int)GetObjectLife());
        Draw_Text_D3D(text, static_cast<int>(pos.x), static_cast<int>(pos.y), D3DCOLOR_ARGB(255, 255, 0, 0));
        maleAntTexture_->Draw(objectPostion.x, objectPostion.y, GetTextureDirection());
    }
}
#pragma endregion


#ifdef  _DEBUG
#include<iostream>
#endif //  _DEBUG
#include "Texture.h"

#pragma region Texture Constructor
CTexture::CTexture(const wchar_t* fileName, const size_t width,
    const size_t height, const int textureSize, const bool isMove) :
    texture_{ nullptr }, textureSize_{ 0 }, isMove_{ isMove }{

    Load_Texture(fileName, 0, width, height);
    texture_ = new BOB[textureSize];

    for (int i = 0; i < textureSize; ++i) {
        if (!Create_BOB32(&texture_[i], 0, 0, 70, height, 0, BOB_ATTR_SINGLE_FRAME)) {
#ifdef  _DEBUG
            std::cout << "Fail Create_BOB32" << std::endl;
#endif //  _DEBUG
        }
        //Frame은 출력할 이미지 칸
        Load_Frame_BOB32(&texture_[i], 0, 0, i, 0, BITMAP_EXTRACT_MODE_CELL);
        texture_[i].attr |= BOB_ATTR_VISIBLE;
    }
    textureSize_ = textureSize;
}


#pragma endregion


CTexture::~CTexture() {
    if (texture_ != nullptr) {
        delete[] texture_;
    }
}

size_t CTexture::GetTextureSize()const {
    return textureSize_;
}

#pragma region Get/Set Texture Position
void CTexture::GetTexturePosition(int& x, int& y) {
    x = texture_->x;
    y = texture_->y;

}

void CTexture::SetTexturePosition(const int x, const int y) {
    texture_->x = x;
    texture_->y = y;
}

#pragma endregion
#pragma region Draw Function Overloading
void CTexture::Draw(const int x, const int y, size_t DrawTextureIndex) {
    if (textureSize_ == 1) {
        texture_->x = x;
        texture_->y = y;
        Draw_BOB32(texture_);
    }
    else {
        texture_[DrawTextureIndex].x = x;
        texture_[DrawTextureIndex].y = y;
        Draw_BOB32(&texture_[DrawTextureIndex]);
    }

}

//This Function Not input x,y
void CTexture::Draw(size_t DrawTextureIndex) {
    if (textureSize_ == 1) {
        Draw_BOB32(texture_);
    }
    else {
        Draw_BOB32(&texture_[DrawTextureIndex]);
    }
}
#pragma endregion 

bool CTexture::CollisionByMouse(const int mouse_x, const int mouse_y) {

    //Change to Screen Position
    D3DXVECTOR3 pos = D3DXVECTOR3((texture_->x - g_left_x) * 65.0f + 8,
        (texture_->y - g_top_y) * 65.0f + 8, 0.0);

    if (pos.x <= mouse_x && mouse_x <= pos.x + texture_->width) {
        if (pos.y <= mouse_y && mouse_y <= pos.y + texture_->height) {
            return true;
        }
    }
    return false;
}

bool CTexture::GetIsMove()const {
    return isMove_;
}

BOB* CTexture::GetTexture()const {
    return texture_;
}

void CTexture::SetTexture(const BOB* texture) {
    texture_ = const_cast<BOB*>(texture);
}
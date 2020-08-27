#pragma once
#include"directX.h"

class CTexture {
private:
    size_t  textureSize_;
    BOB*    texture_;
    bool    isMove_;
public:
    CTexture(const wchar_t* fileName, const size_t width,
        const size_t height, const int textureSize, const bool isMove);
    ~CTexture();

    CTexture() = delete;
    CTexture(const CTexture&) = delete;
    CTexture(const CTexture&&) = delete;
    CTexture& operator=(const CTexture&) = delete;
    CTexture& operator=(const CTexture&&) = delete;

    size_t    GetTextureSize()const;
    void      GetTexturePosition(int& x,int& y);
    void      SetTexturePosition(const int x, const int y);
    void      Draw(const int x, const int y, size_t DrawTextureIndex=0);
    void      Draw(size_t DrawTextureIndex = 0);
    bool      CollisionByMouse(const int mouse_x, const int mouse_y);
};
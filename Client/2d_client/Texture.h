#pragma once
/*
SPDX-FileCopyrightText:     © 2020 DongHyuk Park< SnowFleur0128@gmail.com>
SPDX-License-Identifier:    BSD-3-Clause
More Information visit:     https://snowfleur.tistory.com/159?category=747154
*/
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
    CTexture(CTexture&&) = delete;
    CTexture& operator=(const CTexture&) = delete;
    CTexture& operator=(CTexture&&) = delete;

    size_t    GetTextureSize()const;
    void      GetTexturePosition(int& x,int& y);
    void      SetTexturePosition(const int x, const int y);
    void      Draw(const int x, const int y, size_t DrawTextureIndex=0);
    void      Draw(size_t DrawTextureIndex = 0);
    bool      CollisionByMouse(const int mouse_x, const int mouse_y);
    bool      GetIsMove()const;
    BOB*      GetTexture()const;
    void      SetTexture(const BOB* texture);
};

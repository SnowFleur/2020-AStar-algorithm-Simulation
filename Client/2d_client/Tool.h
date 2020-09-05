#pragma once
/*
SPDX-FileCopyrightText:     © 2020 DongHyuk Park< SnowFleur0128@gamil.com>
SPDX-License-Identifier:    BSD-3-Clause
More Information visit:     https://snowfleur.tistory.com/159?category=747154
*/
#include<memory>
#include"directX.h"
constexpr int TOOLS_X_POSITION = 15;
constexpr int TOOLS_Y_POSITION = 5;

enum TOOL_INDEX {
    TOOL_INDEX_MOUSE = 0,
    TOOL_INDEX_WALL,
    TOOL_INDEX_CHEESE,
    TOOL_INDEX_RESET,
    TOOL_INDEX_START, 
    TOOL_INDEX_NULL,
};

class CToolPimpl;
class CTexture;

using SharedPtrToolPimpl = std::shared_ptr<CToolPimpl>;  //using shared ptr Ctool pimpl
using SharedPtrTexture = std::shared_ptr<CTexture>;      //using shared ptr CTexture

class CTool {
private:
    SharedPtrToolPimpl  pimpl_;
public:
    CTool();
    ~CTool() = default;
    CTool(const CTool&) = delete;
    CTool(CTool&&) = delete;
    CTool& operator=(const CTool&) = delete;
    CTool& operator=(CTool&&) = delete;

    void Draw();
    void Update();
    SharedPtrTexture GetTexture(TOOL_INDEX index)const;
    TOOL_INDEX GetSelectedToolByMouse()const;
    void SetSelectedToolByMouse(TOOL_INDEX index);
};


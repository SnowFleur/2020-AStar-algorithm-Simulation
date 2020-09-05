#pragma once
/*
SPDX-FileCopyrightText:     © 2020 DongHyuk Park< SnowFleur0128@gamil.com>
SPDX-License-Identifier:    BSD-3-Clause
More Information visit:     https://snowfleur.tistory.com/159?category=747154
*/
#include"Global.h"
#include"GameObject.h"

class CMousePimpl;
class CNavigation;
using SharedPtrCMousePimpl = std::shared_ptr<CMousePimpl>;  //using shared ptr CMouse Pimpl

class CMouse:public CGameObject{
private:
    SharedPtrCMousePimpl pimpl_;

    void       SetTextureIndex(Position& current,Position& next);
public:
    CMouse(const int x, const int y);
    ~CMouse() = default;
    CMouse(CMouse&&) = default;
    CMouse() = delete;
    CMouse(const CMouse&) = delete;
    CMouse& operator=(const CMouse&) = delete;
    CMouse& operator=(CMouse&&) = delete;

    void        Update(CWorld& world)override;
    void        StartFindPath(CNavigation& navigation,bool everyFrame)override;
    void        Draw()override;
    void        SetPosition(Position && position);
    Position    GetPosition()const;
    void        DrawPathList()override;
};




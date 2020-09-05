#pragma once
/*
SPDX-FileCopyrightText:     © 2020 DongHyuk Park< SnowFleur0128@gamil.com>
SPDX-License-Identifier:    BSD-3-Clause
More Information visit:     https://snowfleur.tistory.com/159?category=747154
*/
#include<memory>
#include"directX.h"
#include"Navigation.h"

class CWorldPimpl;
class CNavigation;

using SharedPtrPimpl = std::shared_ptr<CWorldPimpl>;       //using Shared Ptr CWorldPimpl
using SharedPtrNavigation = std::shared_ptr <CNavigation>; //using Shared Ptr CNavigation

class CWorld {
private:
    SharedPtrPimpl      pimpl_;
public:
    CWorld();
    ~CWorld() = default;
    CWorld(const CWorld&) = delete;
    CWorld(CWorld&&) = delete;
    CWorld& operator=(const CWorld&) = delete;
    CWorld& operator=(CWorld&&) = delete;

    void                 Update(DWORD time);
    void                 Draw();
    void                 InsertGameObejctByMouse();
    SharedPtrNavigation  GetNavigation()const;
};


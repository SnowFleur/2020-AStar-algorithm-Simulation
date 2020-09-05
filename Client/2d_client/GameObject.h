#pragma once
/*
SPDX-FileCopyrightText:     © 2020 DongHyuk Park< SnowFleur0128@gmail.com>
SPDX-License-Identifier:    BSD-3-Clause
More Information visit:     https://snowfleur.tistory.com/159?category=747154
*/
class CWorld;
class CNavigation;

class CGameObject {
private:
public:
    virtual ~CGameObject() = default;
    virtual void Update(CWorld& world) = 0;
    virtual void StartFindPath(CNavigation& navigation) = 0;
    virtual void SetEveryFrame(const bool everyFrame)=0;
    virtual void Draw() = 0;
    
    virtual void DrawPathList() = 0;
};
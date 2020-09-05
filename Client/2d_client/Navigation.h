#pragma once
/*
SPDX-FileCopyrightText:     © 2020 DongHyuk Park< SnowFleur0128@gamil.com>
SPDX-License-Identifier:    BSD-3-Clause
More Information visit:     https://snowfleur.tistory.com/159?category=747154
*/
#include"Global.h"

constexpr int MAX_WEIGHT = 50000;


enum class CELL_TYPE {
        CT_MOUSE = 1,
        CT_CHEESE = 2,
        CT_WALL = 3,
        CT_GROUND = 4,
};

class Cell {
private:
public:
    CELL_TYPE   cellType_;
    int         weight_;
};

using Cells = std::vector<std::vector<Cell>>;

class CNavigation {
private:
    Cells           cells_;
    Cheeses         cheeses_;
    inline bool     SafetyCheck(const int x, const int y)const;
public:
    CNavigation();
    ~CNavigation() = default;
    CNavigation(const CNavigation&) = default;
    CNavigation(CNavigation&&) = default;
    CNavigation& operator=(const CNavigation&) = default;
    CNavigation& operator=(CNavigation&&) = default;

    void        SetCellType(const int x, const int y,CELL_TYPE type);
    CELL_TYPE   GetCellType(const int x, const int y)const;
    void        SetWeight(const int x, const int y,const int weight);
    int         GetWeight(const int x, const int y)const;
    void        ResetData();
    void        SetCheese(Position&& cheese);
    Cheeses     GetCheeses()const;
};



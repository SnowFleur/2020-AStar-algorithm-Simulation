#pragma once
/*
SPDX-FileCopyrightText:     © 2020 DongHyuk Park< SnowFleur0128@gmail.com>
SPDX-License-Identifier:    BSD-3-Clause
More Information visit:     https://snowfleur.tistory.com/159?category=747154
*/

#include<memory>
#include<vector>
#include<utility>

class  CTexture;
class  CMouse;

using Position          = std::pair<int,int>; //x, y
using SharedPtrTexture  = std::shared_ptr<CTexture>;                     //using shared ptr CTexture
using SharedPtrTextures = std::vector<std::vector< SharedPtrTexture>>;  //using shared ptr in vector
using Cheeses           = std::vector<Position>;

constexpr int MAX_WORLD_X = 14;
constexpr int MAX_WORLD_Y = 10;
constexpr int RESOURCE_HEIGHT = 70;
constexpr int RESOURCE_WIDTH = 770;


/*
Setting Value
*/
//Mouse Speed: LowValue==HighSpeed
// World.cpp -> Update();
static int g_mouseSpeed = 30;

// Select Heuristic Alogirhtm 
// 0: Manhattan Distance, 1: Zero(0) 2: Eucliden Distance 
// 3: OverWeight 4: One(1)
// AStar.cpp->GetHeuristic();
static int g_SelectFindPath = 0;


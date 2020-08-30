#ifdef _DEUBG
#include<iostream>
#endif

#include<queue>
#include<array>
#include"World.h"
#include "AntController.h"
#include"GameObject.h"

CAntController::CAntController() :lastEatDay_{ 0 } {
    antAction_.state_ = ANT_ACTION::ACTION_IDLE;
}

void CAntController::Update(CGameObject& gameobject, CWorld& world) {

    //하루가 지나면 생명력이 감소한다.
    gameobject.DecreaseObjectLife();

    //못 먹은지 5일이 지났다면 생명력이 감소한다.
    if (lastEatDay_ == 5) {
        gameobject.DecreaseObjectLife();
        //10일이 지났다면 한번 더 감소
        if (lastEatDay_ == 10) {
            gameobject.DecreaseObjectLife();
        }
        lastEatDay_ = 0;
    }
    //생명력이 0이라면 삭제
    if (gameobject.GetObjectLife() == 0) {
        antAction_.state_ = ANT_ACTION::ACTION_DIE;
    }
    // 개체수가(일개미+ 수개미) 3/4 이상이라면 추가 2씩감소
    else if (world.GetObjectCount(GAME_OBJECT_TYPE::MALE_ANT) +
        world.GetObjectCount(GAME_OBJECT_TYPE::WORKER_ANT) >= 15) {
        gameobject.DecreaseObjectLife();
        gameobject.DecreaseObjectLife();
    }
    //절반 이상이라면 추가 1씩감소!
    else if (world.GetObjectCount(GAME_OBJECT_TYPE::MALE_ANT) +
        world.GetObjectCount(GAME_OBJECT_TYPE::WORKER_ANT) >= 10) {
        gameobject.DecreaseObjectLife();
    }

    //음식먹을 때 방향좀 보자
    switch (antAction_.state_) {
    case ANT_ACTION::ACTION_IDLE: {
        //현재 내 근처에 음식이 있는가?
        auto foodHanlde = IsArroundFood(world, gameobject);
        if (foodHanlde != nullptr) {

            //음식의 라이프를 1감소
            foodHanlde->DecreaseObjectLife();

            //음식이 0이면
            if (foodHanlde->GetObjectLife() == 0) {
                world.DeleteGameObject(foodHanlde);
            }

            //먹은 후 휴식
            antAction_.state_ = ANT_ACTION::ACTION_BREAK;
        }

        //옆에 음식이 없다. 탐색을 하자
        else {
            antAction_.state_ = ANT_ACTION::ACTION_MOVE;
        }
        break;
    }
    case ANT_ACTION::ACTION_EAT: {
        //현재 내 근처에 음식이 있는가?
        auto foodHanlde = IsArroundFood(world, gameobject);
        if (foodHanlde != nullptr) {

            //음식의 라이프를 1감소
            foodHanlde->DecreaseObjectLife();

            //음식이 0이면
            if (foodHanlde->GetObjectLife() == 0) {
                world.DeleteGameObject(foodHanlde);
            }

            //먹은 후 휴식
            antAction_.state_ = ANT_ACTION::ACTION_BREAK;
        }

        //옆에 음식이 없다. 탐색을 하자
        else {
            antAction_.state_ = ANT_ACTION::ACTION_MOVE;
        }
        break;
    }
    case ANT_ACTION::ACTION_BREAK: {
        //음식을 먹었으니 초기화
        lastEatDay_ = 0;
        //휴식 후에는 다시 식사
        antAction_.state_ = ANT_ACTION::ACTION_EAT;

        break;
    }
    case ANT_ACTION::ACTION_MOVE: {
        //음식을 못먹은 일을 계산해주는 변수
        ++lastEatDay_;

        //다익스트라를 사용해서 가장 가까운놈을 찾자
        CompareDirection(world, gameobject);
        break;
    }
    case ANT_ACTION::ACTION_DIE: {
        //Die
        lastEatDay_ = 0;
        antAction_.state_ = ANT_ACTION::ACTION_IDLE;
        world.DeleteGameObject(&gameobject);
        break;
    }
    default:
        break;
    }

}

CGameObject* CAntController::IsArroundFood(CWorld& world, CGameObject& gameobject) {

    WORD x = gameobject.GetPosition().x;
    WORD y = gameobject.GetPosition().y;

    auto left = world.GetCellsGameObject(x, y, CHECK_CELL_DIRECTION::LEFT);
    if (left != nullptr && left->GetGameObjectType() == GAME_OBJECT_TYPE::FOOD) {
        gameobject.SetTextureDirection(TEXTURE_DIRECTION::TEXTURE_LEFT);
        return left;
    }

    auto right = world.GetCellsGameObject(x, y, CHECK_CELL_DIRECTION::RIGHT);
    if (right != nullptr && right->GetGameObjectType() == GAME_OBJECT_TYPE::FOOD) {
        gameobject.SetTextureDirection(TEXTURE_DIRECTION::TEXTURE_RIGHT);
        return right;
    }

    auto up = world.GetCellsGameObject(x, y, CHECK_CELL_DIRECTION::UP);
    if (up != nullptr && up->GetGameObjectType() == GAME_OBJECT_TYPE::FOOD) {
        gameobject.SetTextureDirection(TEXTURE_DIRECTION::TEXTURE_UP);
        return up;
    }

    auto down = world.GetCellsGameObject(x, y, CHECK_CELL_DIRECTION::DOWN);
    if (down != nullptr && down->GetGameObjectType() == GAME_OBJECT_TYPE::FOOD) {
        gameobject.SetTextureDirection(TEXTURE_DIRECTION::TEXTURE_DOWN);
        return down;
    }

    return nullptr;
}

void CAntController::CompareDirection(CWorld& world, CGameObject& gameobject) {
    Position objectPosition = gameobject.GetPosition();

    if (IsArroundFood(world, gameobject) != nullptr) {
        antAction_.state_ = ANT_ACTION::ACTION_EAT;
    }
    else {
        //4 방향에 대해 가장 가까운 방향으로 이동하자
        std::array<WORD, 4>shortDirectoin{ INF,INF,INF,INF };

        WORD x = objectPosition.x;
        WORD y = objectPosition.y;

        if (world.CheckVoidCell(x, y, CHECK_CELL_DIRECTION::LEFT) == true) {
            shortDirectoin[0] = SearchFood(world, objectPosition.x - 1, objectPosition.y); //왼
        }

        if (world.CheckVoidCell(x, y, CHECK_CELL_DIRECTION::RIGHT) == true) {
            shortDirectoin[1] = SearchFood(world, objectPosition.x + 1, objectPosition.y); //오른
        }

        if (world.CheckVoidCell(x, y, CHECK_CELL_DIRECTION::UP) == true) {
            shortDirectoin[2] = SearchFood(world, objectPosition.x, objectPosition.y - 1); //위
        }

        if (world.CheckVoidCell(x, y, CHECK_CELL_DIRECTION::DOWN) == true) {
            shortDirectoin[3] = SearchFood(world, objectPosition.x, objectPosition.y + 1); //아래
        }

        WORD s{ INF };
        WORD minindex = std::distance(shortDirectoin.begin(), std::min_element(shortDirectoin.begin(), shortDirectoin.end()));
        if (s != shortDirectoin[minindex]) {
            s = minindex;
        }

        if (s == 0) { //왼
            if (world.SwapGameObjectInCells(objectPosition, CHECK_CELL_DIRECTION::LEFT) == true) {
                gameobject.SetPosition(objectPosition.x - 1, objectPosition.y);
                gameobject.SetTextureDirection(TEXTURE_DIRECTION::TEXTURE_LEFT);
            }
#ifdef DEBUG
            else {
                std::cout << "Error:SwapGameObjectInCells()\n";
            }
#endif // DEBUG
        }
        else if (s == 1) { //오른
            if (world.SwapGameObjectInCells(objectPosition, CHECK_CELL_DIRECTION::RIGHT) == true) {
                gameobject.SetPosition(objectPosition.x + 1, objectPosition.y);
                gameobject.SetTextureDirection(TEXTURE_DIRECTION::TEXTURE_RIGHT);
            }
        }
#ifdef DEBUG
        else {
            std::cout << "Error:SwapGameObjectInCells()\n";
        }
#endif // DEBUG
        else if (s == 2) { //위
            if (world.SwapGameObjectInCells(objectPosition, CHECK_CELL_DIRECTION::UP) == true) {
                gameobject.SetPosition(objectPosition.x, objectPosition.y - 1);
                gameobject.SetTextureDirection(TEXTURE_DIRECTION::TEXTURE_UP);
            }
        }
#ifdef DEBUG
        else {
            std::cout << "Error:SwapGameObjectInCells()\n";
        }
#endif // DEBUG
        else if (s == 3) {  //아래 
            if (world.SwapGameObjectInCells(objectPosition, CHECK_CELL_DIRECTION::DOWN) == true) {
                gameobject.SetPosition(objectPosition.x, objectPosition.y + 1);
                gameobject.SetTextureDirection(TEXTURE_DIRECTION::TEXTURE_DOWN);
            }
        }
#ifdef DEBUG
        else {
            std::cout << "Error:SwapGameObjectInCells()\n";
        }
#endif // DEBUG
            } // end around food else 

}

WORD CAntController::SearchFood(CWorld& world, const WORD x, const WORD y) {
    /*
    filed에 Food가 있는지 보고 있으면 탐색을 시작하자
    */
    auto equlIter = world.RangeFindGameObject(GAME_OBJECT_TYPE::FOOD);
    
    std::priority_queue<Node, std::vector<Node>, compare> pq;
    for (auto iter = equlIter.first; iter != equlIter.second; ++iter) {

        pq.emplace(0, Position{ x,y });
        /*일단 이렇게 해보자*/
        int weight[SPACE_SIZE][SPACE_SIZE];
        for (int x = 0; x < SPACE_SIZE; ++x) {
            for (int y = 0; y < SPACE_SIZE; ++y) {
                weight[x][y] = INF;
            }
        }
            weight[x][y] = 0;

        while (pq.empty() == false) {

            auto topNode = pq.top();
            pq.pop();
            int distance = topNode.first;
            int x = topNode.second.x;
            int y = topNode.second.y;

            //왼쪽
            auto left = world.GetCellsGameObject(x, y, CHECK_CELL_DIRECTION::LEFT);
            if (left != nullptr && left->GetGameObjectType() == GAME_OBJECT_TYPE::FOOD) {

                return distance + 1;
            }
            else if(left==nullptr){
                if (x - 1 >= 0 && distance + 1 < weight[x - 1][y]) {
                    weight[x - 1][y] = distance + 1;
                    pq.emplace(distance + 1, Position{ x - 1, y });

                }
            }

            //오른쪽
            auto right = world.GetCellsGameObject(x, y, CHECK_CELL_DIRECTION::RIGHT);
            if (right != nullptr && right->GetGameObjectType() == GAME_OBJECT_TYPE::FOOD) {
                return distance + 1;
            }
            else if (right == nullptr) {
                if (x + 1 < SPACE_SIZE && distance + 1 < weight[x + 1][y]) {
                    weight[x + 1][y] = distance + 1;
                    pq.emplace(distance + 1, Position{ x + 1, y });

                }
            }

            //위
            auto up = world.GetCellsGameObject(x, y, CHECK_CELL_DIRECTION::UP);
            if (up != nullptr && up->GetGameObjectType() == GAME_OBJECT_TYPE::FOOD) {
                return distance + 1;
            }
            else if (up == nullptr) {
                if (y - 1 >= 0 && distance + 1 < weight[x][y - 1]) {
                    weight[x][y - 1] = distance + 1;
                    pq.emplace(distance + 1, Position{ x,y - 1 });

                }
            }
            //아래
            auto down = world.GetCellsGameObject(x, y, CHECK_CELL_DIRECTION::DOWN);
            if (down != nullptr && down->GetGameObjectType() == GAME_OBJECT_TYPE::FOOD) {
                return distance + 1;
            }
            else if (down == nullptr) {
                if (y + 1 < SPACE_SIZE && distance + 1 < weight[x][y + 1]) {
                    weight[x][y + 1] = distance + 1;
                    pq.emplace(distance + 1, Position{ x, y + 1 });

                }
            }

        } //end 우선순위규

        //Rest it
        pq = std::priority_queue<Node, std::vector<Node>, compare>();
    }// end equl_range
    return INF;
}


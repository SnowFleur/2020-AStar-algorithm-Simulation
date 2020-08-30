#ifdef _DEBUG
#include<iostream>
#endif // _DEBUG
#include"World.h"
#include"GameObject.h"
#include "QueenController.h"
#include"Food.h"

CQueenController::CQueenController() {
    queenAction_.state_ = QUEEN_ACTION::ACTION_CREATED_ANT;
}

void CQueenController::Update(CGameObject& gameobject, CWorld& world) {
    
    //하루가 지나면 생명력이 감소한다.
    gameobject.DecreaseObjectLife();

    //생명력이 0이면 삭제
    if (gameobject.GetObjectLife() == 0) {
        queenAction_.state_ = QUEEN_ACTION::ACTION_DIE;
    }
    //생명력이 0이 아니면서 개체수가 3/4이하라면 출산
    else {
        // 개체수가 3/4(수개미+일개미) 이상이라면 3씩 감소!
        if (world.GetObjectCount(GAME_OBJECT_TYPE::MALE_ANT) +
            world.GetObjectCount(GAME_OBJECT_TYPE::WORKER_ANT) >= 15) {
            gameobject.DecreaseObjectLife();
            gameobject.DecreaseObjectLife();
            //생성 중지
            queenAction_.state_ = QUEEN_ACTION::ACTION_IDLE;
        }
        //절반 이상이라면 2씩 감소!
        else if (world.GetObjectCount() >= 10) {
            gameobject.DecreaseObjectLife();
            //출산 시작
            queenAction_.state_ = QUEEN_ACTION::ACTION_CREATED_ANT;
        }
        else {
            //출산 시작
            queenAction_.state_ = QUEEN_ACTION::ACTION_CREATED_ANT;
        }
    }

    switch (queenAction_.state_) {
    case QUEEN_ACTION::ACTION_IDLE: {
        break;
    }
    case QUEEN_ACTION::ACTION_CREATED_ANT: {
#pragma region Create Worker Ant
        //일개미는 10일당 한마리 생성
        if (world.GetDays() % WORKER_ANT_CREATED_DATE == 0) {
            Position pos = gameobject.GetPosition();
            if (world.CheckVoidCell(pos.x, pos.y, CHECK_CELL_DIRECTION::LEFT) == true) {
                pos.x -= 1;

                world.CreateGameObject(Position{ pos }, WORKER_LIFE, GAME_OBJECT_TYPE::WORKER_ANT);
            }
            else if (world.CheckVoidCell(pos.x, pos.y, CHECK_CELL_DIRECTION::RIGHT) == true) {
                pos.x += 1;
                world.CreateGameObject(Position{ pos }, WORKER_LIFE, GAME_OBJECT_TYPE::WORKER_ANT);
            }
            else if (world.CheckVoidCell(pos.x, pos.y, CHECK_CELL_DIRECTION::DOWN) == true) {
                pos.y += 1;
                world.CreateGameObject(Position{ pos }, WORKER_LIFE, GAME_OBJECT_TYPE::WORKER_ANT);
            }
            else if (world.CheckVoidCell(pos.x, pos.y, CHECK_CELL_DIRECTION::UP) == true) {
                pos.y -= 1;
                world.CreateGameObject(Position{ pos }, WORKER_LIFE, GAME_OBJECT_TYPE::WORKER_ANT);
            }

        }
#pragma endregion
#pragma region Create Male Ant
        //수개미는 5일당 한마리 생성
        if (world.GetDays() % MALE_ANT_CREATED_DATE == 0) {

            Position pos = gameobject.GetPosition();
            if (world.CheckVoidCell(pos.x, pos.y, CHECK_CELL_DIRECTION::LEFT) == true) {
                pos.x -= 1;
                world.CreateGameObject(Position{ pos }, MALE_LIFE, GAME_OBJECT_TYPE::MALE_ANT);
            }
            else if (world.CheckVoidCell(pos.x, pos.y, CHECK_CELL_DIRECTION::RIGHT) == true) {
                pos.x += 1;

                world.CreateGameObject(Position{ pos }, MALE_LIFE, GAME_OBJECT_TYPE::MALE_ANT);
            }
            else if (world.CheckVoidCell(pos.x, pos.y, CHECK_CELL_DIRECTION::DOWN) == true) {
                pos.y += 1;
                world.CreateGameObject(Position{ pos }, MALE_LIFE, GAME_OBJECT_TYPE::MALE_ANT);
            }
            else if (world.CheckVoidCell(pos.x, pos.y, CHECK_CELL_DIRECTION::UP) == true) {
                pos.y -= 1;
                world.CreateGameObject(Position{ pos }, MALE_LIFE, GAME_OBJECT_TYPE::MALE_ANT);
            }
        }
#pragma endregion
        break;
    }
    case QUEEN_ACTION::ACTION_DIE: {
        queenAction_.state_ = QUEEN_ACTION::ACTION_IDLE;
        world.DeleteGameObject(&gameobject);
        break;
    }
    default:
        break;
    }
}
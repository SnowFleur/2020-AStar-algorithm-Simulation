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
    
    //�Ϸ簡 ������ ������� �����Ѵ�.
    gameobject.DecreaseObjectLife();

    //������� 0�̸� ����
    if (gameobject.GetObjectLife() == 0) {
        queenAction_.state_ = QUEEN_ACTION::ACTION_DIE;
    }
    //������� 0�� �ƴϸ鼭 ��ü���� 3/4���϶�� ���
    else {
        // ��ü���� 3/4(������+�ϰ���) �̻��̶�� 3�� ����!
        if (world.GetObjectCount(GAME_OBJECT_TYPE::MALE_ANT) +
            world.GetObjectCount(GAME_OBJECT_TYPE::WORKER_ANT) >= 15) {
            gameobject.DecreaseObjectLife();
            gameobject.DecreaseObjectLife();
            //���� ����
            queenAction_.state_ = QUEEN_ACTION::ACTION_IDLE;
        }
        //���� �̻��̶�� 2�� ����!
        else if (world.GetObjectCount() >= 10) {
            gameobject.DecreaseObjectLife();
            //��� ����
            queenAction_.state_ = QUEEN_ACTION::ACTION_CREATED_ANT;
        }
        else {
            //��� ����
            queenAction_.state_ = QUEEN_ACTION::ACTION_CREATED_ANT;
        }
    }

    switch (queenAction_.state_) {
    case QUEEN_ACTION::ACTION_IDLE: {
        break;
    }
    case QUEEN_ACTION::ACTION_CREATED_ANT: {
#pragma region Create Worker Ant
        //�ϰ��̴� 10�ϴ� �Ѹ��� ����
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
        //�����̴� 5�ϴ� �Ѹ��� ����
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
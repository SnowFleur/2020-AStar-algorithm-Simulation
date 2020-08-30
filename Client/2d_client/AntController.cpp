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

    //�Ϸ簡 ������ ������� �����Ѵ�.
    gameobject.DecreaseObjectLife();

    //�� ������ 5���� �����ٸ� ������� �����Ѵ�.
    if (lastEatDay_ == 5) {
        gameobject.DecreaseObjectLife();
        //10���� �����ٸ� �ѹ� �� ����
        if (lastEatDay_ == 10) {
            gameobject.DecreaseObjectLife();
        }
        lastEatDay_ = 0;
    }
    //������� 0�̶�� ����
    if (gameobject.GetObjectLife() == 0) {
        antAction_.state_ = ANT_ACTION::ACTION_DIE;
    }
    // ��ü����(�ϰ���+ ������) 3/4 �̻��̶�� �߰� 2������
    else if (world.GetObjectCount(GAME_OBJECT_TYPE::MALE_ANT) +
        world.GetObjectCount(GAME_OBJECT_TYPE::WORKER_ANT) >= 15) {
        gameobject.DecreaseObjectLife();
        gameobject.DecreaseObjectLife();
    }
    //���� �̻��̶�� �߰� 1������!
    else if (world.GetObjectCount(GAME_OBJECT_TYPE::MALE_ANT) +
        world.GetObjectCount(GAME_OBJECT_TYPE::WORKER_ANT) >= 10) {
        gameobject.DecreaseObjectLife();
    }

    //���ĸ��� �� ������ ����
    switch (antAction_.state_) {
    case ANT_ACTION::ACTION_IDLE: {
        //���� �� ��ó�� ������ �ִ°�?
        auto foodHanlde = IsArroundFood(world, gameobject);
        if (foodHanlde != nullptr) {

            //������ �������� 1����
            foodHanlde->DecreaseObjectLife();

            //������ 0�̸�
            if (foodHanlde->GetObjectLife() == 0) {
                world.DeleteGameObject(foodHanlde);
            }

            //���� �� �޽�
            antAction_.state_ = ANT_ACTION::ACTION_BREAK;
        }

        //���� ������ ����. Ž���� ����
        else {
            antAction_.state_ = ANT_ACTION::ACTION_MOVE;
        }
        break;
    }
    case ANT_ACTION::ACTION_EAT: {
        //���� �� ��ó�� ������ �ִ°�?
        auto foodHanlde = IsArroundFood(world, gameobject);
        if (foodHanlde != nullptr) {

            //������ �������� 1����
            foodHanlde->DecreaseObjectLife();

            //������ 0�̸�
            if (foodHanlde->GetObjectLife() == 0) {
                world.DeleteGameObject(foodHanlde);
            }

            //���� �� �޽�
            antAction_.state_ = ANT_ACTION::ACTION_BREAK;
        }

        //���� ������ ����. Ž���� ����
        else {
            antAction_.state_ = ANT_ACTION::ACTION_MOVE;
        }
        break;
    }
    case ANT_ACTION::ACTION_BREAK: {
        //������ �Ծ����� �ʱ�ȭ
        lastEatDay_ = 0;
        //�޽� �Ŀ��� �ٽ� �Ļ�
        antAction_.state_ = ANT_ACTION::ACTION_EAT;

        break;
    }
    case ANT_ACTION::ACTION_MOVE: {
        //������ ������ ���� ������ִ� ����
        ++lastEatDay_;

        //���ͽ�Ʈ�� ����ؼ� ���� �������� ã��
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
        //4 ���⿡ ���� ���� ����� �������� �̵�����
        std::array<WORD, 4>shortDirectoin{ INF,INF,INF,INF };

        WORD x = objectPosition.x;
        WORD y = objectPosition.y;

        if (world.CheckVoidCell(x, y, CHECK_CELL_DIRECTION::LEFT) == true) {
            shortDirectoin[0] = SearchFood(world, objectPosition.x - 1, objectPosition.y); //��
        }

        if (world.CheckVoidCell(x, y, CHECK_CELL_DIRECTION::RIGHT) == true) {
            shortDirectoin[1] = SearchFood(world, objectPosition.x + 1, objectPosition.y); //����
        }

        if (world.CheckVoidCell(x, y, CHECK_CELL_DIRECTION::UP) == true) {
            shortDirectoin[2] = SearchFood(world, objectPosition.x, objectPosition.y - 1); //��
        }

        if (world.CheckVoidCell(x, y, CHECK_CELL_DIRECTION::DOWN) == true) {
            shortDirectoin[3] = SearchFood(world, objectPosition.x, objectPosition.y + 1); //�Ʒ�
        }

        WORD s{ INF };
        WORD minindex = std::distance(shortDirectoin.begin(), std::min_element(shortDirectoin.begin(), shortDirectoin.end()));
        if (s != shortDirectoin[minindex]) {
            s = minindex;
        }

        if (s == 0) { //��
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
        else if (s == 1) { //����
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
        else if (s == 2) { //��
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
        else if (s == 3) {  //�Ʒ� 
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
    filed�� Food�� �ִ��� ���� ������ Ž���� ��������
    */
    auto equlIter = world.RangeFindGameObject(GAME_OBJECT_TYPE::FOOD);
    
    std::priority_queue<Node, std::vector<Node>, compare> pq;
    for (auto iter = equlIter.first; iter != equlIter.second; ++iter) {

        pq.emplace(0, Position{ x,y });
        /*�ϴ� �̷��� �غ���*/
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

            //����
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

            //������
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

            //��
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
            //�Ʒ�
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

        } //end �켱������

        //Rest it
        pq = std::priority_queue<Node, std::vector<Node>, compare>();
    }// end equl_range
    return INF;
}


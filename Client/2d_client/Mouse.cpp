#ifdef _DEBUG
#include<iostream>
#endif // _DEBUG

#include<queue>
#include "Mouse.h"
#include"Texture.h"
#include"Astar.h"
#include"World.h"

using ShortpathPair = std::pair<size_t, Astar::ShortPath>;

class CMousePimpl {
public:
    SharedPtrTexture        texture_;
    size_t                  currentTextureIndex_;
    Position                position_;
    CAstar                  astar_;
    Astar::ShortPath        pathList;
    Astar::ShortPathIter    pathIter;
    bool                    everyFrame_; //Every Frame PathFinding
};

CMouse::CMouse(const int x, const int y) :pimpl_(std::make_shared<CMousePimpl>()) {
    pimpl_->texture_ = std::make_shared<CTexture>
        (L"resource/Mouse.PNG", RESOURCE_WIDTH, RESOURCE_HEIGHT, 4, false);
    //Init Position
    pimpl_->position_.first = x;
    pimpl_->position_.second = y;
    pimpl_->currentTextureIndex_ = 0;
    //Init Path
    pimpl_->pathIter = pimpl_->pathList.rbegin();
    pimpl_->everyFrame_ = false;
}

void CMouse::Update(CWorld& world) {
    //Run Every Time FindPath
    if (pimpl_->everyFrame_ == true) {
        StartFindPath(*(world.GetNavigation().get()));
    }
    if (pimpl_->pathIter != pimpl_->pathList.rend()) {
        //Next Position
        auto next = *pimpl_->pathIter;
        // Current Position
        auto current = pimpl_->position_;

        //�� �̹��� ����(Change to Mouse texture)
        SetTextureIndex(current, next);

        //���� ��ġ�� �������� �ƴ϶�� �� �̵�
        if ((pimpl_->pathIter + 1) != pimpl_->pathList.rend()) {
            //Set Infor of Navigation
            auto nextCellType = world.GetNavigation()->GetCellType(next.first, next.second);
            //�̹� ���� ���� �ٲ���ٸ� ������
            if (nextCellType != CELL_TYPE::CT_MOUSE) {
                world.GetNavigation()->SetCellType(next.first, next.second, CELL_TYPE::CT_MOUSE);
                world.GetNavigation()->SetCellType(current.first, current.second, nextCellType);
                pimpl_->position_ = next;
            }
        }
        //���� ��ġ�� ġ���� �̵����� �ʰ� �� �ڸ����� ����Ȱ�� �����Ѵ�.
        else {
            world.GetNavigation()->SetCellType(current.first, current.second, CELL_TYPE::CT_MOUSE);
        }

        //Next Iter
        pimpl_->pathIter++;
    }
}

void CMouse::Draw() {
    //Draw x, y, DrawIndex
    pimpl_->texture_->Draw(pimpl_->position_.first, pimpl_->position_.second,
        pimpl_->currentTextureIndex_);
}

void CMouse::SetPosition(Position&& position) {
    pimpl_->position_.first = position.first;
    pimpl_->position_.second = position.second;
}

Position CMouse::GetPosition()const {
    return pimpl_->position_;
}

void CMouse::SetTextureIndex(Position& current, Position& next) {
    int x = current.first - next.first;
    int y = current.second - next.second;

    //Left =2 
    if (x > 0) {
        pimpl_->currentTextureIndex_ = 2;
    }
    //Right=1
    else if (x < 0) {
        pimpl_->currentTextureIndex_ = 1;
    }
    //Down=3
    else if (y < 0) {
        pimpl_->currentTextureIndex_ = 3;
    }
    //UP =0
    else {
        pimpl_->currentTextureIndex_ = 0;
    }
}

void CMouse::StartFindPath(CNavigation& navigation) {
    if (navigation.GetCheeses().size() == 1) {
        auto cheese = *navigation.GetCheeses().begin();
        pimpl_->pathList = pimpl_->astar_.StartFindPath(pimpl_->position_, cheese, navigation);
        pimpl_->pathIter = pimpl_->pathList.rbegin();
    }
    else {
        // ���忡 �ִ� ġ�����ŭ �ݺ����� ����.
        // Mouse: Start, Cheese: End, Navigation: moveable status
        // ���� �� ���� ��θ� ã�Ҵٸ� �� ��η� �ٲ۴�.
        std::priority_queue<ShortpathPair, std::vector<ShortpathPair>, std::greater<>>pq;
        //��� ġ� ���鼭 ���� �Ÿ��� ª�� pathList�� �����´�.
        for (auto& cheese : navigation.GetCheeses()) {
            auto pathList = pimpl_->astar_.StartFindPath(pimpl_->position_, cheese, navigation);
            //����� 0�̶�� ���� ��� �켱���� ���ķ� ���� ���� ����
            if (pathList.size() != 0) {
                pq.emplace(pathList.size(), pathList);
            }
        }
        if (pq.empty() == false) {
            pimpl_->pathList = pq.top().second;
            pimpl_->pathIter = pimpl_->pathList.rbegin();
        }
    }
}

void CMouse::SetEveryFrame(const bool everyFrame) {
    pimpl_->everyFrame_ = everyFrame;
}

void CMouse::DrawPathList() {
    pimpl_->astar_.SetDrawPath(pimpl_->pathList);
    pimpl_->astar_.Draw();
}

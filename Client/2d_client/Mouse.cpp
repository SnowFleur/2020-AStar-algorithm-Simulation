#ifdef _DEBUG
#include<iostream>
#endif // _DEBUG

#include "Mouse.h"
#include"Texture.h"
#include"Astar.h"
#include"World.h"

class CMousePimpl {
public:
    SharedPtrTexture        texture_;
    size_t                  currentTextureIndex_;
    Position                position_;
    CAstar                  astar_;
    Astar::ShortPath        pathList;
    Astar::ShortPathIter    pathIter;
};

CMouse::CMouse(const int x, const int y):pimpl_(std::make_shared<CMousePimpl>()){
    pimpl_->texture_ = std::make_shared<CTexture>
        (L"resource/Mouse.PNG", RESOURCE_WIDTH, RESOURCE_HEIGHT, 4, false);
    //Init Position
    pimpl_->position_.first = x;
    pimpl_->position_.second = y;
    pimpl_->currentTextureIndex_ = 0;
    //Init Path
    pimpl_->pathIter = pimpl_->pathList.rbegin();
}

void CMouse::Update(CWorld& world) {

    if (pimpl_->pathIter != pimpl_->pathList.rend()) {
        //Next Position
        auto next = *pimpl_->pathIter;
        // Current Position
        auto current = pimpl_->position_;

        //Set Infor of Navigation
        auto nextCellType = world.GetNavigation()->GetCellType(next.first, next.second);

        /*
        이거의 문제점은 치즈가 있을 떄 치즈의 위치를 바꾼다는 잠재적 문제도 있음
        치즈일경우 바꾸지 말던가
        치즈를 따로 빼던지 해야함
        CELL은 이동가능여부 false/true만 참고하고 찾는거는 cheese에서 찾을까?

        */

        if (nextCellType != CELL_TYPE::CT_CHEESE) {
            world.GetNavigation()->SetCellType(next.first, next.second, CELL_TYPE::CT_MOUSE);
            world.GetNavigation()->SetCellType(current.first, current.second, nextCellType);
        }

        //마우스 이미지
        SetTextureIndex(current, next);
        //Move image only at the end
        if ((pimpl_->pathIter + 1) != pimpl_->pathList.rend()) {
            pimpl_->position_ = next;

#ifdef _DEBUG
            std::cout <<"Update: "<<pimpl_->position_.first << ", " << pimpl_->position_.second << "\n";
#endif // _DEBUG

        }
        //Next Iter
        pimpl_->pathIter++;
    }
}

void CMouse::Draw() {
    //Draw x, y, DrawIndex
    pimpl_->texture_->Draw(pimpl_->position_.first,pimpl_->position_.second, 
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

void CMouse::StartFindPath(CNavigation& navigation,bool everyFrame) {

    if (everyFrame == true) {
        pimpl_->pathList = pimpl_->astar_.StartFindPath(pimpl_->position_, navigation);
        pimpl_->pathIter = pimpl_->pathList.rbegin();
#ifdef _DEBUG
        std::cout << pimpl_->position_.first << ", " << pimpl_->position_.second << "\n";
#endif // _DEBUG
    }
    else {
        if (pimpl_->pathList.size() == 0) {
            pimpl_->pathList = pimpl_->astar_.StartFindPath(pimpl_->position_, navigation);
            pimpl_->pathIter = pimpl_->pathList.rbegin();
        }
    }
}

void CMouse::DrawPathList() {
    pimpl_->astar_.Draw();
}

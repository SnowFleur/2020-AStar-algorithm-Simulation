#ifdef _DEBUG
#include<iostream>
#endif // DEBUG

#include<queue>
#include<algorithm>
#include "Astar.h"
#include"Texture.h"
#include"Navigation.h"
#include"Node.h"

namespace Astar {
    using PairData = std::pair<int, CNodeSharePtr>;   //weight, Node
    using PairPosition = std::pair<int, Position>;
    using OpenList = std::priority_queue<PairData, std::vector<PairData>, Compare>;
    using CloseList = std::vector<Position>;
}

struct Compare {
public:
    Compare() = delete;
    bool operator()(const Astar::PairData& lhs, const Astar::PairData& rhs) {
        return lhs.first > rhs.first;
    }
};

class CAStarPimpl {
public:
    Position            direction_[4];
    Astar::ShortPath    shortPath_;
    Astar::OpenList     openList_;
    Astar::CloseList    closeList_;
#ifdef _DEBUG
#else
    Astar::ShortPath    drawPath_;
#endif // _DEBUG

};

CAstar::CAstar() :pimpl_(std::make_shared< CAStarPimpl>()) {
    //left
    pimpl_->direction_[0].first = -1;
    pimpl_->direction_[0].second = 0;
    //right
    pimpl_->direction_[1].first = +1;
    pimpl_->direction_[1].second = 0;
    //up
    pimpl_->direction_[2].first = 0;
    pimpl_->direction_[2].second = -1;
    //down
    pimpl_->direction_[3].first = 0;
    pimpl_->direction_[3].second = +1;
}

int CAstar::GetHeuristic(Position lhs, Position rhs) {

    // x= first y= second
    switch (g_SelectFindPath) {
    case 0: {
        //Manhattan Distance
        return abs(lhs.first - rhs.first) + abs(lhs.second - rhs.second);
        break;
    }
    case 1: {
        //Zero 
        return 0;
        break;
    }
    case 2: {
        // Euclidean distance
        return sqrt(pow(lhs.first - rhs.first, 2) + pow(lhs.second - rhs.second, 2));
        break;
    }
    case 3: {
        // Over Weight
        return sqrt(pow(lhs.first - rhs.first, 2) + pow(lhs.second - rhs.second, 2)) * 50;
        break;
    }
    case 4: {
        //One
        return 1;
        break;
    }
    default:
        break;
    }
}

bool CAstar::CheckVaildByNode(Position&& currentPosition, CNavigation& navigation) {
    int x = currentPosition.first;
    int y = currentPosition.second;

    //범위 안에 있으며
    if ((x >= 0 && x < MAX_WORLD_X) && (y >= 0 && y < MAX_WORLD_Y)) {
        //Close List에 없고
        bool isCLoseList = CheckByCloseList(std::move(currentPosition));
        //이동이 가능하다면
        auto type = navigation.GetCellType(x, y);
        if (!isCLoseList
            && type != CELL_TYPE::CT_WALL
            && type != CELL_TYPE::CT_MOUSE) {
            return true;
        }
    }
    return false;
}

bool CAstar::CheckByCloseList(Position&& currentPosition) {
    auto iter = std::find(pimpl_->closeList_.begin(), pimpl_->closeList_.end(), currentPosition);
    if (iter != pimpl_->closeList_.end())
        return true;
    return false;
}

void CAstar::ResetData()const {
    //Best Way Use Memory Pool
    //Reset shortpath, closeList, openList
    pimpl_->shortPath_.clear();
    pimpl_->closeList_.clear();
    pimpl_->openList_ = Astar::OpenList();
#ifdef _DEBUG
#else
    pimpl_->drawPath_.clear();
#endif // _DEBUG
}

Astar::ShortPath CAstar::StartFindPath(Position mouse, Position cheese, CNavigation navigation) {

    ResetData();

    //시작 지점을 Open List에 넣는다.
    pimpl_->openList_.emplace(0, new CNode{ mouse,nullptr });
    navigation.SetCellType(cheese.first, cheese.second, CELL_TYPE::CT_CHEESE);

    while (pimpl_->openList_.empty() == false) {
        //First: Weight Second: Position
        auto topPosition = pimpl_->openList_.top().second->position_;
        auto topNode = pimpl_->openList_.top().second;
        auto topWeight = pimpl_->openList_.top().first;
        pimpl_->openList_.pop();

        //Add topVaule Position in Clost List
        pimpl_->closeList_.emplace_back(topPosition);

        int tx = topPosition.first;
        int ty = topPosition.second;

        //if (cheese.first == tx && cheese.second == ty) {
        if (navigation.GetCellType(tx, ty) == CELL_TYPE::CT_CHEESE) {
            while (topNode != nullptr) {
                pimpl_->shortPath_.emplace_back(topNode->position_);
                topNode = topNode->next_;
            }
#ifdef _DEBUG
            std::cout << "Open  List Size" << pimpl_->openList_.size() << std::endl;
            std::cout << "Close List Size" << pimpl_->closeList_.size() << std::endl;
#endif // _DEBUG
            pimpl_->shortPath_.pop_back();
            return pimpl_->shortPath_;
        }

        //Left->Right->Up->Down
        for (int i = 0; i < 4; ++i) {
            int x = tx + pimpl_->direction_[i].first;
            int y = ty + pimpl_->direction_[i].second;
            //유효 여부 체크
            if (CheckVaildByNode(Position{ x,y }, navigation) == true) {
                //시작 지점부터 현재까지의 값 g(x) 
                int g = topWeight + ADD_WEIGHT;
                //현재 위치(상)에서 포지션까지의 값 h(x)
                int h = GetHeuristic(Position{ x,y }, Position{ cheese });
                //f(x)=g(x)+h(x)
                int f = g + h;
                //old weight
                int oldWeight = navigation.GetWeight(x, y);

                if (oldWeight > f) {
                    navigation.SetWeight(x, y, f);
                    pimpl_->openList_.emplace(Astar::PairData{ f,new CNode{Position{x,y},topNode} });
                }
            }
        }
    } //End  While Close List
#ifdef _DEBUG
    std::cout << "Not Find Cheese\n";
#endif // _DEBUG
    return pimpl_->shortPath_;
}

void CAstar::SetDrawPath(Astar::ShortPath& drawPath) {
#ifdef _DEBUG
#else
    pimpl_->drawPath_ = drawPath;
#endif // _DEBUG
}

void CAstar::Draw() {
#ifdef _DEBUG
#else //Release
    int count{};
    for (auto iter = pimpl_->drawPath_.rbegin(); iter != pimpl_->drawPath_.rend(); ++iter) {

        D3DXVECTOR3 pos = D3DXVECTOR3((iter->first - g_left_x) * 65.0f + 8,
            (iter->second - g_top_y) * 65.0f + 8, 0.0);

        wchar_t text[10]{};
        wsprintf(text, L"%d", (int)++count);
        Draw_Text_D3D(text, static_cast<int>(pos.x + 20), static_cast<int>(pos.y + 20), D3DCOLOR_ARGB(255, 255, 0, 0));
    }
#endif // _DEBUG
}
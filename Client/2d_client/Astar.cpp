#ifdef _DEBUG
#include<iostream>
#endif // DEBUG

#include<queue>
#include<unordered_set>
#include<algorithm>
#include<deque>
#include "Astar.h"
#include"Texture.h"
#include"Navigation.h"

using AStarPair         = std::pair<int, Position>;   //weight, Position
using AstarOpenList     = std::priority_queue<AStarPair, std::vector<AStarPair>, Compare>;
using AStartCloseList   = std::vector<Position>;
using AStartPath        = std::deque<AStarPair>;
using AStartNavigation  = std::vector <std::vector<CNavigation>>;

struct Compare {
public:
    Compare() = delete;
    bool operator()(const AStarPair& lhs, const AStarPair& rhs) {
        return lhs.first > rhs.first;
    }
};


// 이동가능여부, 해당 위치의 weight

class CAStarPimpl {
public:
    AStartCloseList     closeList_;
    AstarOpenList       openList_;
    AStartPath          pathList_;
    AStartNavigation    navigation_;
    Position            direction_[4];
};

CAstar::CAstar() :pimpl_(std::make_shared< CAStarPimpl>()) {
    pimpl_->navigation_.assign(MAX_WORLD_X, std::vector<CNavigation>(MAX_WORLD_Y));
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
    //Use Heuristic Manhattan distance
    // x= first y= second
    return abs(lhs.first - rhs.first) + abs(lhs.second - rhs.second);
    //return 0;
    //return abs(lhs.first - rhs.first) + abs(lhs.second - rhs.second)*2;
    //return sqrt(pow(lhs.first - rhs.first, 2) + pow(lhs.second - rhs.second, 2));
}

bool CAstar::CheckVaildByNode(Position&& currentPosition) {

    int x = currentPosition.first;
    int y = currentPosition.second;

    //범위 안에 있으며
    if ((x >= 0 && x < MAX_WORLD_X) && (y >= 0 && y < MAX_WORLD_Y)) {
        //Close List에 없고
        bool isCLoseList = CheckByCloseList(std::move(currentPosition));
        //이동이 가능하다면
        bool isMove = pimpl_->navigation_[x][y].GetMoveAble();
        if (!isCLoseList && isMove) {
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

void CAstar::SetIsMoveByNavigation(const int x, const int y, const bool isMove) {
    pimpl_->navigation_[x][y].SetMoveAble(isMove);
}

void CAstar::ResetByNavigation()const {

    for (int x = 0; x < pimpl_->navigation_.size(); ++x) {
        for (int y = 0; y < pimpl_->navigation_[x].size(); ++y) {
            pimpl_->navigation_[x][y].ResetData();
        }
    }

}

void CAstar::StartFindPath(VectorInObject& mouses, VectorInObject& cheeses, SharedPtrTextures& cells) {

    //일단 한개의 마우스와 한개의 치즈의 위치를 받아온다.
    auto  mouse = *mouses.begin();
    auto  cheese = *cheeses.begin();

    //시작 지점을 Open List에 넣는다.
    pimpl_->openList_.emplace(0, std::move(mouse));

    //나중에 재시작이 편하게 하도록 레퍼런스인자가 아니라 카피로 해야함
    //pimpl_->pathList_.clear();
    //ResetByNavigation();
    //Close와 Open도 초기화를 해야함

    while (pimpl_->openList_.empty() == false) {
        auto topValue = pimpl_->openList_.top();
        pimpl_->openList_.pop();

        //Save To Path
        pimpl_->pathList_.push_front(topValue);

        //Add topVaule Position in Clost List
        pimpl_->closeList_.emplace_back(topValue.second);

        int tx = topValue.second.first;
        int ty = topValue.second.second;


        if (tx == cheese.first && ty == cheese.second) {
#ifdef _DEBUG
            std::cout << "Find Cheese\n";
#endif // DEBUG
            break;
        }
        for (int i = 0; i < 4; ++i) {
            int x = tx + pimpl_->direction_[i].first;
            int y = ty + pimpl_->direction_[i].second;
            //유효 여부 체크
            if (CheckVaildByNode(Position{ x,y }) == true) {
                //시작 지점부터 현재까지의 값 g(x) 
                int g = topValue.first + ADD_WEIGHT;
                //현재 위치(상)에서 포지션까지의 값 h(x)
                int h = GetHeuristic(Position{ x,y }, Position{ cheese });
                //f(x)=g(x)+h(x)
                int f = g + h;
                //old weight
                int oldWeight = pimpl_->navigation_[x][y].GetWeight();

                if (oldWeight > f) {
                    pimpl_->navigation_[x][y].SetWeight(f);
                    pimpl_->openList_.emplace(AStarPair{ f,Position{x,y} });
                }
            }
        }
    } //End  While Close List
}

void CAstar:: Draw() {
    int count{};
    for (const auto& path : pimpl_->pathList_) {

        D3DXVECTOR3 pos = D3DXVECTOR3((path.second.first - g_left_x) * 65.0f + 8,
            (path.second.second- g_top_y) * 65.0f + 8, 0.0);

        wchar_t text[10]{};
        //wsprintf(text, L"%d", (int)++count);
        wsprintf(text, L"%d", (int)path.first);

        Draw_Text_D3D(text, static_cast<int>(pos.x), static_cast<int>(pos.y), D3DCOLOR_ARGB(255, 255, 0, 0));

    }
}
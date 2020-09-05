#pragma once
/*
SPDX-FileCopyrightText:     © 2020 DongHyuk Park< SnowFleur0128@gmail.com>
SPDX-License-Identifier:    BSD-3-Clause
More Information visit:     https://snowfleur.tistory.com/159?category=747154
*/
#include"Global.h"

class   CAStarPimpl;
class   CNavigation;
struct  Compare;

constexpr int ADD_WEIGHT = 1;

namespace Astar {
    using ShortPath     = std::vector<Position>;
    using ShortPathIter = ShortPath::reverse_iterator;
}

using SharedPtrAStartPimpl = std::shared_ptr<CAStarPimpl>; //using shared ptr CAstartPimpl

class CAstar {
private:
    SharedPtrAStartPimpl   pimpl_;
    int                    GetHeuristic(Position lhs, Position rhs);
    bool                   CheckVaildByNode(Position&& currentPosition, CNavigation& navigation);
    bool                   CheckByCloseList(Position&& currentPosition);

public:
    CAstar();
    ~CAstar() = default;
    CAstar(const CAstar&) = delete;
    CAstar(CAstar&&) = delete;
    CAstar& operator=(const CAstar&) = delete;
    CAstar& operator=(CAstar&&) = delete;

    void                ResetData()const;
    Astar::ShortPath    StartFindPath(Position mouse, Position cheese, CNavigation navigation);
    void                SetDrawPath(Astar::ShortPath& drawPath);
    void                Draw();
};


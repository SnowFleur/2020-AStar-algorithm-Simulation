#pragma once
#include"Global.h"

class  CAStarPimpl;
struct Compare;

constexpr int ADD_WEIGHT = 1;

//enum class DIRECTION {
//    UP      = 1,
//    DOWN    = 2,
//    LEFT    = 3,
//    RIGHT   = 4,
//};

using SharedPtrAStartPimpl = std::shared_ptr<CAStarPimpl>; //using shared ptr CAstartPimpl

class CAstar{
private:
    SharedPtrAStartPimpl   pimpl_;
    int                    GetHeuristic(Position lhs, Position rhs);
    bool                   CheckVaildByNode(Position&& currentPosition);
    bool                   CheckByCloseList(Position&& currentPosition);
    
public:
    CAstar();
    ~CAstar() = default;
    CAstar(const CAstar&) = delete;
    CAstar(CAstar&&) = delete;
    CAstar& operator=(const CAstar&) = delete;
    CAstar& operator=( CAstar&&) = delete;

    void    SetIsMoveByNavigation(const int x, const int y, const bool isMove);
    void    ResetByNavigation()const;
    void    StartFindPath(VectorInObject& mouses, VectorInObject& cheeses, SharedPtrTextures& cells);
    void    Draw();
};


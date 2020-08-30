#pragma once

constexpr int MAX_WEIGHT = 500;


class CNavigation{
private:
    bool isMove_;
    int  weight_;
public:
    CNavigation();
    ~CNavigation() = default;
    CNavigation(const CNavigation&) = default;
    CNavigation(CNavigation&&) = default;
    CNavigation& operator=(const CNavigation&) = default;
    CNavigation& operator=(CNavigation&&) = default;

    void    SetMoveAble(bool isMove);
    bool    GetMoveAble()const;
    void    SetWeight(int weight);
    int     GetWeight()const;
    void    ResetData();
};


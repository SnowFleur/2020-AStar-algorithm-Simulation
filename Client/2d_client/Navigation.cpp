#include "Navigation.h"

CNavigation::CNavigation() {
    cells_.assign(MAX_WORLD_X, std::vector<Cell>(MAX_WORLD_Y,
        Cell{ CELL_TYPE::CT_GROUND,MAX_WEIGHT }));
}

bool CNavigation::SafetyCheck(const int x, const int y)const {
    if ((0 <= x && x <= MAX_WORLD_X) && (0 <= y && y <= MAX_WORLD_Y))
        return true;
    else
        return false;
}

void CNavigation::SetCellType(const int x, const int y, CELL_TYPE type) {
    if (SafetyCheck(x, y) == true) {
        cells_[x][y].cellType_ = type;
    }
}

CELL_TYPE CNavigation::GetCellType(const int x, const int y)const {
    if (SafetyCheck(x, y) == true) {
        return cells_[x][y].cellType_;
    }
}

void CNavigation::SetWeight(const int x, const int y, const int weight) {
    if ((0 <= x && x <= MAX_WORLD_X) && (0 <= y && y <= MAX_WORLD_Y)) {
        cells_[x][y].weight_ = weight;
    }
}

int CNavigation::GetWeight(const int x, const int y)const {
    if ((0 <= x && x <= MAX_WORLD_X) && (0 <= y && y <= MAX_WORLD_Y)) {
        return cells_[x][y].weight_;
    }
}

void CNavigation::ResetData() {
    for (int x = 0; x < MAX_WORLD_X; ++x) {
        for (int y = 0; y < MAX_WORLD_Y; ++y) {
            cells_[x][y].cellType_ = CELL_TYPE::CT_GROUND;
            cells_[x][y].weight_ = MAX_WEIGHT;
        }
    }
    cheeses_.clear();
}

#pragma region Get/Set Cheese
void CNavigation::SetCheese(Position&& cheese) {
    cheeses_.emplace_back(cheese);
}

Cheeses CNavigation::GetCheeses()const {
    return cheeses_;
}
#pragma endregion
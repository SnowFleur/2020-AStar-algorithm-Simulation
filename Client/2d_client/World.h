#pragma once
#include<memory>
#include"directX.h"

constexpr int MAX_WORLD_X       = 14;
constexpr int MAX_WORLD_Y       = 10;
constexpr int RESOURCE_HEIGHT   = 70;
constexpr int RESOURCE_WIDTH    = 770;

class CWorldPimpl;
using SharedPtrPimpl = std::shared_ptr<CWorldPimpl>;

class CWorld {
private:
    SharedPtrPimpl      pimpl_;
public:
    CWorld();
    ~CWorld() = default;
    CWorld(const CWorld&) = delete;
    CWorld(const CWorld&&) = delete;
    CWorld& operator=(const CWorld&) = delete;
    CWorld& operator=(const CWorld&&) = delete;

    void               Update(DWORD time);
    void               Draw();
    void               Reset();
    void               InsertGameObejctByMouse();

};



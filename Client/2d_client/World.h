#pragma once
#include<memory>
#include"directX.h"


class CWorldPimpl;
using SharedPtrPimpl = std::shared_ptr<CWorldPimpl>;

class CWorld {
private:
    SharedPtrPimpl      pimpl_;
public:
    CWorld();
    ~CWorld() = default;
    CWorld(const CWorld&) = delete;
    CWorld(CWorld&&) = delete;
    CWorld& operator=(const CWorld&) = delete;
    CWorld& operator=(CWorld&&) = delete;

    void               Update(DWORD time);
    void               Draw();
    void               Reset();
    void               InsertGameObejctByMouse();

};


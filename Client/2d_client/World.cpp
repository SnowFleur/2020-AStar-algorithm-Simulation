#ifdef _DEBUG
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#include<iostream>
#endif // DEBUG

#include"global.h"
#include "World.h" //���� class
#include"Texture.h" //�ؽ�ó class
#include"Tool.h" //�� class
#include"MouseController.h" //���콺 Static Class
#include"Mouse.h" //Mouse Class
#include"Astar.h"

using SharedPtrTool         = std::shared_ptr<CTool>;           //using shared ptr CTool
using SharedPtrAStar        = std::shared_ptr<CAstar>;          //using Shared Ptr CAstar
using GameObjects           = std::vector<CMouse>;

class CWorldPimpl {
public:
    SharedPtrTool        tool_;
    SharedPtrNavigation  navigation_;
    SharedPtrTextures    cells_;
    SharedPtrTexture     groundTexture_;
    GameObjects          gameobjects_;
};

CWorld::CWorld() :pimpl_(std::make_shared<CWorldPimpl>()) {
    //Init Tool
    pimpl_->tool_ = std::make_unique<CTool>();
    //Init Texture
    pimpl_->groundTexture_ = std::make_shared<CTexture>
        (L"resource/backGround.PNG", RESOURCE_WIDTH, RESOURCE_HEIGHT, 4, true);
    //Init Cells
    pimpl_->cells_.assign(
        MAX_WORLD_X, std::vector<SharedPtrTexture>(MAX_WORLD_Y, pimpl_->groundTexture_));
    //Init Navigation
    pimpl_->navigation_ = std::make_shared<CNavigation>();
}

void CWorld::Update(DWORD time) {
    pimpl_->tool_->Update();
    InsertGameObejctByMouse();

    //GameObjects Update
    if (time % g_mouseSpeed == 0) {
        if (pimpl_->gameobjects_.size() == 1) {
            for (auto& gameobject : pimpl_->gameobjects_) {
                gameobject.Update(*this);
            }
        }
        else {
            for (auto& gameobject : pimpl_->gameobjects_) {
                gameobject.StartFindPath(*(pimpl_->navigation_.get()), true);
                gameobject.Update(*this);
            }
        }
    }
    Draw();
}

void CWorld::Draw() {
    g_pSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);
    for (int x = 0; x < MAX_WORLD_X; ++x) {
        for (int y = 0; y < MAX_WORLD_Y; ++y) {
            pimpl_->cells_[x][y]->Draw(x, y);
        }
    }
    //draw tool 
    pimpl_->tool_->Draw();
    g_pSprite->End();

    g_pSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);
    //GameObjects Update
    
    for (auto& gameobject : pimpl_->gameobjects_) {
        gameobject.Draw();
        //gameobject.DrawPathList();

        if (pimpl_->gameobjects_.begin() != pimpl_->gameobjects_.end()) {
            auto game = pimpl_->gameobjects_.begin();
            game->DrawPathList();
        }
    }
    g_pSprite->End();
}

void CWorld::InsertGameObejctByMouse() {

    switch (pimpl_->tool_->GetSelectedToolByMouse()) {
    case TOOL_INDEX::TOOL_INDEX_MOUSE: {
        //Chage to World Location
        int x, y;
        CMouseController::GetHandle()->GetMousePosition(x, y);
        x = (x - 8) / 65;
        y = (y - 8) / 65;

        //���߿� �����ʿ� �ʹ� �Ҿ�����
        if (x >= MAX_WORLD_X || y >= MAX_WORLD_Y)break;

        //������ ���� ���콺�� �� ��ġ�� �ֱ� ������ ��Ӵ�����.
        CMouseController::GetHandle()->SetMousePosition(100000, 1000000);
       
        // 1. ���콺 ��ǥ�� ȭ������� �ű��.
        // 2. ��Ե� �������� ȣ���ϰ� �Ѵ�.

        //Set Mouse Position By Navigation
        pimpl_->navigation_->SetCellType(x, y, CELL_TYPE::CT_MOUSE);

        //Set Mouse
        pimpl_->gameobjects_.emplace_back(CMouse{ x,y });
        break;
    }
    case TOOL_INDEX::TOOL_INDEX_WALL: {
        //Chage to World Location
        int x, y;
        CMouseController::GetHandle()->GetMousePosition(x, y);
        x = (x - 8) / 65;
        y = (y - 8) / 65;

        //���߿� �����ʿ� �ʹ� �Ҿ�����
        if (x >= MAX_WORLD_X || y >= MAX_WORLD_Y)break;

        //Shared Ptr�� �̿��ؼ� �ؽ��Ĵ� ����
        pimpl_->cells_[x][y] = pimpl_->tool_->GetTexture(TOOL_INDEX_WALL);

        //Set Move availability
        pimpl_->navigation_->SetCellType(x, y, CELL_TYPE::CT_WALL);

        break;
    }
    case TOOL_INDEX::TOOL_INDEX_CHEESE: {
        //Chage to World Location
        int x, y;
        CMouseController::GetHandle()->GetMousePosition(x, y);
        x = (x - 8) / 65;
        y = (y - 8) / 65;

        //���߿� �����ʿ� �ʹ� �Ҿ�����
        if (x >= MAX_WORLD_X || y >= MAX_WORLD_Y)break;

        CMouseController::GetHandle()->SetMousePosition(100000, 1000000);


        //Shared Ptr�� �̿��ؼ� �ؽ��Ĵ� ����
        pimpl_->cells_[x][y] = pimpl_->tool_->GetTexture(TOOL_INDEX_CHEESE);

        //Set Cheese Position By navigation
        pimpl_->navigation_->SetCheese(Position{ x,y });
        pimpl_->navigation_->SetCellType(x, y, CELL_TYPE::CT_CHEESE);


        break;
    }
    case TOOL_INDEX::TOOL_INDEX_RESET: {
        for (int x = 0; x < MAX_WORLD_X; ++x) {
            for (int y = 0; y < MAX_WORLD_Y; ++y) {
                pimpl_->cells_[x][y] = pimpl_->groundTexture_;
            }
        }
        pimpl_->navigation_->ResetData();
        pimpl_->gameobjects_.clear();
        break;
    }
    case TOOL_INDEX::TOOL_INDEX_START: {
        // 1. dynamic_cast 
        // 2. Add StartFindPath
        for (auto& gameobject : pimpl_->gameobjects_) {
            gameobject.StartFindPath(*(pimpl_->navigation_.get()),false);
        }
        pimpl_->tool_->SetSelectedToolByMouse(TOOL_INDEX::TOOL_INDEX_NULL);
        break;
    }
    default:
        break;
    }
}

SharedPtrNavigation CWorld::GetNavigation()const {
    return pimpl_->navigation_;
}

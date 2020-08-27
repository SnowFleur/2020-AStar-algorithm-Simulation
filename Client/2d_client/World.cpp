#ifdef _DEBUG
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#include<iostream>
#endif // DEBUG

#include"Texture.h" //텍스처 class
#include "World.h" //월드 class
#include"Tool.h" //툴 class
#include"MouseController.h" //마우스 Static Class

using SharedPtrTool = std::shared_ptr<CTool>;           //using shared ptr CTool
using SharedPtrTexture = std::shared_ptr<CTexture>;     //using shared ptr CTexture

class CWorldPimpl {
public:
    SharedPtrTool       tool_;
    SharedPtrTexture    cells_[MAX_WORLD_X][MAX_WORLD_Y];
    SharedPtrTexture    groundTexture_;
};


CWorld::CWorld() :pimpl_(std::make_shared<CWorldPimpl>()){

    //Init Smart Ptrs 
    pimpl_->tool_ = std::make_unique<CTool>();

    pimpl_->groundTexture_ = std::make_shared<CTexture>
        (L"resource/backGround.PNG", RESOURCE_WIDTH, RESOURCE_HEIGHT, 4,true);

    for (int x = 0; x < MAX_WORLD_X; ++x) {
        for (int y = 0; y < MAX_WORLD_Y; ++y) {
            pimpl_->cells_[x][y] = pimpl_->groundTexture_;
        }
    }
}

void CWorld::Update(DWORD time) {
    pimpl_->tool_->Update();
    InsertGameObejctByMouse();
    Draw();
}

void CWorld::Draw() {
    g_pSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);
    for (int x = 0; x < MAX_WORLD_X; ++x) {
        for (int y = 0; y < MAX_WORLD_Y; ++y) {

            pimpl_->cells_[x][y]->Draw(x, y);
        }
    }

    //darw tool 
    pimpl_->tool_->Draw();


    g_pSprite->End();
    //playerController_->Draw();
}

void CWorld::InsertGameObejctByMouse() {

    switch (CMouseController::GetCurrentTool()) {
    case CURRENT_TOOL::CURRENT_TOOL_MOUSE: {
            
        //Chage to World Location
        int x, y;
        CMouseController::GetControllerByMouse(x, y);
        x = (x - 8) / 65;
        y = (y - 8) / 65;
        
        //나중에 수정필요 너무 불안정함
        if (x >= MAX_WORLD_X || y >= MAX_WORLD_Y)break;

        //Shared Ptr을 이용해서 텍스쳐는 공유
        pimpl_->cells_[x][y] = pimpl_->tool_->GetTexture(TOOL_INDEX_MOUSE);

        break;
    }
    case CURRENT_TOOL::CURRENT_TOOL_WALL: {

        //Chage to World Location
        int x, y;
        CMouseController::GetControllerByMouse(x, y);
        x = (x - 8) / 65;
        y = (y - 8) / 65;

        //나중에 수정필요 너무 불안정함
        if (x >= MAX_WORLD_X || y >= MAX_WORLD_Y)break;

        //Shared Ptr을 이용해서 텍스쳐는 공유
        pimpl_->cells_[x][y] = pimpl_->tool_->GetTexture(TOOL_INDEX_WALL);

        break;
    }
    case CURRENT_TOOL::CURRENT_TOOL_CHEESE: {
        //Chage to World Location
        int x, y;
        CMouseController::GetControllerByMouse(x, y);
        x = (x - 8) / 65;
        y = (y - 8) / 65;

        //나중에 수정필요 너무 불안정함
        if (x >= MAX_WORLD_X || y >= MAX_WORLD_Y)break;

        //Shared Ptr을 이용해서 텍스쳐는 공유
        pimpl_->cells_[x][y] = pimpl_->tool_->GetTexture(TOOL_INDEX_CHEESE);

        break;
    }
    case CURRENT_TOOL::CURRENT_TOOL_RESET: {
        for (int x = 0; x < MAX_WORLD_X; ++x) {
            for (int y = 0; y < MAX_WORLD_Y; ++y) {
                pimpl_->cells_[x][y] = pimpl_->groundTexture_;
            }
        }
        break;
    }
    default:
        break;
    }
}


/*

20.08.19 인수인계
1. 일단 컨트롤러 이미지 움직이는거 까지 했음

2. 오른쪽에 파이프라인 쥐, 치즈를 납두고 셀렉트로 스페이스바를 누르면 이동할 수 있게
3. 배치할 수 있게 맵을 일단 만들자
4. 맵 배치가 완료되면 이제 다익스트라 끌고오고 시뮬레이션 하면 끝

배치까지 가능하면 블로그 하나 올리고
그 다음 시뮬되면 올려서 2개로 나누자.


20;08.23 인수인계
1. 최적화는 마지막까지의 과제


4. 컨트롤러 넣고 A*돌리는대 (먹는거 까지는 필요없고 도착만 보여주면됨)
5. 방향에 따른 Texture전환이 필요하니까 텍스쳐 4개를 가지고있는 놈이 필요해보임


*/
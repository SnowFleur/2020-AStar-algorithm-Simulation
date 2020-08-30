#ifdef _DEBUG
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#include<iostream>
#endif // DEBUG

#include "World.h" //월드 class
#include"Texture.h" //텍스처 class
#include"Tool.h" //툴 class
#include"MouseController.h" //마우스 Static Class
#include"Astar.h" //길찾기 class
#include"global.h" //using files

using SharedPtrTool = std::shared_ptr<CTool>;                       //using shared ptr CTool
using SharedPtrAStar = std::shared_ptr<CAstar>;                      //using Shared Ptr CAstar

class CWorldPimpl {
public:
    SharedPtrTool       tool_;
    SharedPtrAStar      aStarHandle_;
    SharedPtrTextures   cells_;
    SharedPtrTexture    groundTexture_;
    VectorInObject      mouses_;
    VectorInObject      cheese_;
};

CWorld::CWorld() :pimpl_(std::make_shared<CWorldPimpl>()) {
    //Init Smart Ptrs 
    pimpl_->tool_ = std::make_unique<CTool>();
    pimpl_->groundTexture_ = std::make_shared<CTexture>
        (L"resource/backGround.PNG", RESOURCE_WIDTH, RESOURCE_HEIGHT, 4, true);
    pimpl_->aStarHandle_ = std::make_shared<CAstar>();
    pimpl_->cells_.assign(
        MAX_WORLD_X, std::vector<SharedPtrTexture>(MAX_WORLD_Y, pimpl_->groundTexture_));
}

void CWorld::Update(DWORD time) {
    
    //tool Update
    pimpl_->tool_->Update();
    //World Update
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
    
    g_pSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);
    pimpl_->aStarHandle_->Draw();
    g_pSprite->End();

}

void CWorld::InsertGameObejctByMouse() {

    switch (pimpl_->tool_->GetSelectedToolByMouse()){
    case TOOL_INDEX::TOOL_INDEX_MOUSE:{ 
        //Chage to World Location
        int x, y;
        CMouseController::GetHandle()->GetMousePosition(x, y);
        x = (x - 8) / 65;
        y = (y - 8) / 65;

        //나중에 수정필요 너무 불안정함
        if (x >= MAX_WORLD_X || y >= MAX_WORLD_Y)break;

        //Shared Ptr을 이용해서 텍스쳐는 공유
        pimpl_->cells_[x][y] = pimpl_->tool_->GetTexture(TOOL_INDEX_MOUSE);

        //탐색을 위해 Vector에 저장 (이것도 네비게이션으로 옮기자)
        pimpl_->mouses_.emplace_back(Position{ x,y });

        //같은 쥐가 있을 때 이동여부 false 해야할 수 있음

        break; 
    }
    case TOOL_INDEX::TOOL_INDEX_WALL: { 
        //Chage to World Location
        int x, y;
        CMouseController::GetHandle()->GetMousePosition(x, y);
        x = (x - 8) / 65;
        y = (y - 8) / 65;

        //나중에 수정필요 너무 불안정함
        if (x >= MAX_WORLD_X || y >= MAX_WORLD_Y)break;

        //Shared Ptr을 이용해서 텍스쳐는 공유
        pimpl_->cells_[x][y] = pimpl_->tool_->GetTexture(TOOL_INDEX_WALL);

        //Set Move availability
        pimpl_->aStarHandle_->SetIsMoveByNavigation(x, y, false);
        break;
    }
    case TOOL_INDEX::TOOL_INDEX_CHEESE: { 
        //Chage to World Location
        int x, y;
        CMouseController::GetHandle()->GetMousePosition(x, y);
        x = (x - 8) / 65;
        y = (y - 8) / 65;

        //나중에 수정필요 너무 불안정함
        if (x >= MAX_WORLD_X || y >= MAX_WORLD_Y)break;

        //Shared Ptr을 이용해서 텍스쳐는 공유
        pimpl_->cells_[x][y] = pimpl_->tool_->GetTexture(TOOL_INDEX_CHEESE);

        //탐색을 위해 Vector에 저장 (나중에 수정)
        pimpl_->cheese_.emplace_back(Position{ x,y });
        break;
    }
    case TOOL_INDEX::TOOL_INDEX_RESET: { 
        for (int x = 0; x < MAX_WORLD_X; ++x) {
            for (int y = 0; y < MAX_WORLD_Y; ++y) {
                pimpl_->cells_[x][y] = pimpl_->groundTexture_;
            }
        }
        break;
    }
    case TOOL_INDEX::TOOL_INDEX_START: { 
        //Start를 누르면
        pimpl_->aStarHandle_->StartFindPath(pimpl_->mouses_, pimpl_->cheese_, pimpl_->cells_);

        pimpl_->tool_->SetSelectedToolByMouse(TOOL_INDEX::TOOL_INDEX_NULL);

        break;
    }
    default:
        break;
    }


}


/*

old x,y와 비교해서 바뀐게 없다면 continue?
근데 이거는 다시 Start를 눌렀다고 했을 때 작동안할 수 있다.
Click을 했을 때만 다시 마우스를 올리면 

1. 마우스를 클릭했을 떄 해동하는 좌표 저장해놓고 마우스에서 손을 때면 초기화
2. Start만 그렇게 
툴들은 상태를 저장해놓고 계속 클릭하면 그거를 호출하게
Start와 Reset은 따로 

Click했을 때 호출해서 충돌체크 하고 상태 저장 (매프레임 체크 X)







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


1. Path List좀 어떻게 해봐야할듯
2. 이동
3. 최적화


*/


/*


    switch (CMouseController::GetHandle()->GetMouseState()) {
    case MOUSE_STATE::CLICK_MOUSE: {

        //Chage to World Location
        int x, y;
        CMouseController::GetHandle()->GetMousePosition(x, y);
        x = (x - 8) / 65;
        y = (y - 8) / 65;

        //나중에 수정필요 너무 불안정함
        if (x >= MAX_WORLD_X || y >= MAX_WORLD_Y)break;

        //Shared Ptr을 이용해서 텍스쳐는 공유
        pimpl_->cells_[x][y] = pimpl_->tool_->GetTexture(TOOL_INDEX_MOUSE);

        //탐색을 위해 Vector에 저장 (이것도 네비게이션으로 옮기자)
        pimpl_->mouses_.emplace_back(Position{ x,y });

        //같은 쥐가 있을 떄 이동여부 false 해야할 수 있음

        break;
    }
    case MOUSE_STATE::CLICK_WALL: {

        //Chage to World Location
        int x, y;
        CMouseController::GetHandle()->GetMousePosition(x, y);
        x = (x - 8) / 65;
        y = (y - 8) / 65;

        //나중에 수정필요 너무 불안정함
        if (x >= MAX_WORLD_X || y >= MAX_WORLD_Y)break;

        //Shared Ptr을 이용해서 텍스쳐는 공유
        pimpl_->cells_[x][y] = pimpl_->tool_->GetTexture(TOOL_INDEX_WALL);

        //Set Move availability
        pimpl_->aStarHandle_->SetIsMoveByNavigation(x, y, false);

        break;
    }
    case MOUSE_STATE::CLICK_CHEESE: {
        //Chage to World Location
        int x, y;
        CMouseController::GetHandle()->GetMousePosition(x, y);
        x = (x - 8) / 65;
        y = (y - 8) / 65;

        //나중에 수정필요 너무 불안정함
        if (x >= MAX_WORLD_X || y >= MAX_WORLD_Y)break;

        //Shared Ptr을 이용해서 텍스쳐는 공유
        pimpl_->cells_[x][y] = pimpl_->tool_->GetTexture(TOOL_INDEX_CHEESE);

        //탐색을 위해 Vector에 저장 (나중에 수정)
        pimpl_->cheese_.emplace_back(Position{ x,y });

        break;
    }
    case MOUSE_STATE::CLICK_RESET: {
        for (int x = 0; x < MAX_WORLD_X; ++x) {
            for (int y = 0; y < MAX_WORLD_Y; ++y) {
                pimpl_->cells_[x][y] = pimpl_->groundTexture_;
            }
        }

        break;
    }
    case MOUSE_STATE::CLICK_START: {
        //Start를 누르면
        pimpl_->aStarHandle_->StartFindPath(pimpl_->mouses_, pimpl_->cheese_, pimpl_->cells_);

        break;
    }
    default:
        break;
    }
*/
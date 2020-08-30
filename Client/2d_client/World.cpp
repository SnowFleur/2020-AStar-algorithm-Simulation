#ifdef _DEBUG
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#include<iostream>
#endif // DEBUG

#include "World.h" //���� class
#include"Texture.h" //�ؽ�ó class
#include"Tool.h" //�� class
#include"MouseController.h" //���콺 Static Class
#include"Astar.h" //��ã�� class
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

        //���߿� �����ʿ� �ʹ� �Ҿ�����
        if (x >= MAX_WORLD_X || y >= MAX_WORLD_Y)break;

        //Shared Ptr�� �̿��ؼ� �ؽ��Ĵ� ����
        pimpl_->cells_[x][y] = pimpl_->tool_->GetTexture(TOOL_INDEX_MOUSE);

        //Ž���� ���� Vector�� ���� (�̰͵� �׺���̼����� �ű���)
        pimpl_->mouses_.emplace_back(Position{ x,y });

        //���� �㰡 ���� �� �̵����� false �ؾ��� �� ����

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
        pimpl_->aStarHandle_->SetIsMoveByNavigation(x, y, false);
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

        //Shared Ptr�� �̿��ؼ� �ؽ��Ĵ� ����
        pimpl_->cells_[x][y] = pimpl_->tool_->GetTexture(TOOL_INDEX_CHEESE);

        //Ž���� ���� Vector�� ���� (���߿� ����)
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
        //Start�� ������
        pimpl_->aStarHandle_->StartFindPath(pimpl_->mouses_, pimpl_->cheese_, pimpl_->cells_);

        pimpl_->tool_->SetSelectedToolByMouse(TOOL_INDEX::TOOL_INDEX_NULL);

        break;
    }
    default:
        break;
    }


}


/*

old x,y�� ���ؼ� �ٲ�� ���ٸ� continue?
�ٵ� �̰Ŵ� �ٽ� Start�� �����ٰ� ���� �� �۵����� �� �ִ�.
Click�� ���� ���� �ٽ� ���콺�� �ø��� 

1. ���콺�� Ŭ������ �� �ص��ϴ� ��ǥ �����س��� ���콺���� ���� ���� �ʱ�ȭ
2. Start�� �׷��� 
������ ���¸� �����س��� ��� Ŭ���ϸ� �װŸ� ȣ���ϰ�
Start�� Reset�� ���� 

Click���� �� ȣ���ؼ� �浹üũ �ϰ� ���� ���� (�������� üũ X)







20.08.19 �μ��ΰ�
1. �ϴ� ��Ʈ�ѷ� �̹��� �����̴°� ���� ����

2. �����ʿ� ���������� ��, ġ� ���ΰ� ����Ʈ�� �����̽��ٸ� ������ �̵��� �� �ְ�
3. ��ġ�� �� �ְ� ���� �ϴ� ������
4. �� ��ġ�� �Ϸ�Ǹ� ���� ���ͽ�Ʈ�� ������� �ùķ��̼� �ϸ� ��

��ġ���� �����ϸ� ��α� �ϳ� �ø���
�� ���� �ùĵǸ� �÷��� 2���� ������.


20;08.23 �μ��ΰ�
1. ����ȭ�� ������������ ����


4. ��Ʈ�ѷ� �ְ� A*�����´� (�Դ°� ������ �ʿ���� ������ �����ָ��)
5. ���⿡ ���� Texture��ȯ�� �ʿ��ϴϱ� �ؽ��� 4���� �������ִ� ���� �ʿ��غ���


1. Path List�� ��� �غ����ҵ�
2. �̵�
3. ����ȭ


*/


/*


    switch (CMouseController::GetHandle()->GetMouseState()) {
    case MOUSE_STATE::CLICK_MOUSE: {

        //Chage to World Location
        int x, y;
        CMouseController::GetHandle()->GetMousePosition(x, y);
        x = (x - 8) / 65;
        y = (y - 8) / 65;

        //���߿� �����ʿ� �ʹ� �Ҿ�����
        if (x >= MAX_WORLD_X || y >= MAX_WORLD_Y)break;

        //Shared Ptr�� �̿��ؼ� �ؽ��Ĵ� ����
        pimpl_->cells_[x][y] = pimpl_->tool_->GetTexture(TOOL_INDEX_MOUSE);

        //Ž���� ���� Vector�� ���� (�̰͵� �׺���̼����� �ű���)
        pimpl_->mouses_.emplace_back(Position{ x,y });

        //���� �㰡 ���� �� �̵����� false �ؾ��� �� ����

        break;
    }
    case MOUSE_STATE::CLICK_WALL: {

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
        pimpl_->aStarHandle_->SetIsMoveByNavigation(x, y, false);

        break;
    }
    case MOUSE_STATE::CLICK_CHEESE: {
        //Chage to World Location
        int x, y;
        CMouseController::GetHandle()->GetMousePosition(x, y);
        x = (x - 8) / 65;
        y = (y - 8) / 65;

        //���߿� �����ʿ� �ʹ� �Ҿ�����
        if (x >= MAX_WORLD_X || y >= MAX_WORLD_Y)break;

        //Shared Ptr�� �̿��ؼ� �ؽ��Ĵ� ����
        pimpl_->cells_[x][y] = pimpl_->tool_->GetTexture(TOOL_INDEX_CHEESE);

        //Ž���� ���� Vector�� ���� (���߿� ����)
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
        //Start�� ������
        pimpl_->aStarHandle_->StartFindPath(pimpl_->mouses_, pimpl_->cheese_, pimpl_->cells_);

        break;
    }
    default:
        break;
    }
*/
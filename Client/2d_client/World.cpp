#ifdef _DEBUG
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#include<iostream>
#endif // DEBUG

#include"Texture.h" //�ؽ�ó class
#include "World.h" //���� class
#include"Tool.h" //�� class
#include"MouseController.h" //���콺 Static Class

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
        
        //���߿� �����ʿ� �ʹ� �Ҿ�����
        if (x >= MAX_WORLD_X || y >= MAX_WORLD_Y)break;

        //Shared Ptr�� �̿��ؼ� �ؽ��Ĵ� ����
        pimpl_->cells_[x][y] = pimpl_->tool_->GetTexture(TOOL_INDEX_MOUSE);

        break;
    }
    case CURRENT_TOOL::CURRENT_TOOL_WALL: {

        //Chage to World Location
        int x, y;
        CMouseController::GetControllerByMouse(x, y);
        x = (x - 8) / 65;
        y = (y - 8) / 65;

        //���߿� �����ʿ� �ʹ� �Ҿ�����
        if (x >= MAX_WORLD_X || y >= MAX_WORLD_Y)break;

        //Shared Ptr�� �̿��ؼ� �ؽ��Ĵ� ����
        pimpl_->cells_[x][y] = pimpl_->tool_->GetTexture(TOOL_INDEX_WALL);

        break;
    }
    case CURRENT_TOOL::CURRENT_TOOL_CHEESE: {
        //Chage to World Location
        int x, y;
        CMouseController::GetControllerByMouse(x, y);
        x = (x - 8) / 65;
        y = (y - 8) / 65;

        //���߿� �����ʿ� �ʹ� �Ҿ�����
        if (x >= MAX_WORLD_X || y >= MAX_WORLD_Y)break;

        //Shared Ptr�� �̿��ؼ� �ؽ��Ĵ� ����
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


*/
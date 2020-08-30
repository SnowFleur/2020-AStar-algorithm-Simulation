#ifdef _DEBUG
#include<iostream>
#endif // _DEBUG

#include<vector> //Vector 

#include"World.h"
#include"Texture.h"
#include "Tool.h"
#include"MouseController.h"
#include"Global.h"

using Tools = std::vector<std::shared_ptr<CTexture>>;   //using uniqueu ptr vector
using UniquePtrTexture = std::unique_ptr<CTexture>;     //using unique ptr CTexture

//Pimpl
class CToolPimpl {
public:
    UniquePtrTexture    selectTexture_;     //Select Texture
    Tools               tools_;             //Tool   Textures
    TOOL_INDEX          selectToolByMouse_; // Used Mouse Selected Tool
};

CTool::CTool() :pimpl_(std::make_shared< CToolPimpl>()) {
    //Set Tool Image (Mouse, Wall, Cheese, Reset )
    pimpl_->tools_.emplace_back(
        std::make_shared<CTexture>(L"resource/Mouse.PNG", RESOURCE_WIDTH, RESOURCE_HEIGHT, 4,false));

    pimpl_->tools_.emplace_back(
        std::make_shared<CTexture>(L"resource/Wall.PNG", RESOURCE_WIDTH, RESOURCE_HEIGHT, 1, false));

    pimpl_->tools_.emplace_back(
        std::make_shared<CTexture>(L"resource/cheese.PNG", RESOURCE_WIDTH, RESOURCE_HEIGHT, 1,false));

    pimpl_->tools_.emplace_back(
        std::make_shared<CTexture>(L"resource/Reset.PNG", RESOURCE_WIDTH, RESOURCE_HEIGHT, 1, false));

    pimpl_->tools_.emplace_back(
        std::make_shared<CTexture>(L"resource/Start.PNG", RESOURCE_WIDTH, RESOURCE_HEIGHT, 1, false));

    //Set Select Image
    pimpl_->selectTexture_ = std::make_unique<CTexture>
        (L"resource/Select.PNG", RESOURCE_WIDTH, RESOURCE_HEIGHT, 1,false);
    pimpl_->selectToolByMouse_ = TOOL_INDEX_NULL;
}

void CTool::Update() {

    if (CMouseController::GetHandle()->GetMouseState() == MOUSE_STATE::CLICK_UP)
        return;

    int x, y;
    CMouseController::GetHandle()->GetMousePosition(x, y);

    //Collision By tools
    if (pimpl_->tools_[TOOL_INDEX_MOUSE]->CollisionByMouse(x, y) == true) {
        pimpl_->tools_[TOOL_INDEX_MOUSE]->GetTexturePosition(x, y);
        pimpl_->selectTexture_->SetTexturePosition(x, y);
        pimpl_->selectToolByMouse_ = TOOL_INDEX_MOUSE;
#ifdef _DEBUG
        std::cout << "Collision is Mouse \n";
#endif
    }
    else if (pimpl_->tools_[TOOL_INDEX_WALL]->CollisionByMouse(x, y) == true) {
        pimpl_->tools_[TOOL_INDEX_WALL]->GetTexturePosition(x, y);
        pimpl_->selectTexture_->SetTexturePosition(x, y);
        pimpl_->selectToolByMouse_ = TOOL_INDEX_WALL;
#ifdef _DEBUG
        std::cout << "Collision is Wall \n";
#endif 
    }
    else if (pimpl_->tools_[TOOL_INDEX_CHEESE]->CollisionByMouse(x, y) == true) {
        pimpl_->tools_[TOOL_INDEX_CHEESE]->GetTexturePosition(x, y);
        pimpl_->selectTexture_->SetTexturePosition(x, y);
        pimpl_->selectToolByMouse_ = TOOL_INDEX_CHEESE;
#ifdef _DEBUG
        std::cout << "Collision is Cheese \n";
#endif
    }
    else if (pimpl_->tools_[TOOL_INDEX_RESET]->CollisionByMouse(x, y) == true) {
        pimpl_->tools_[TOOL_INDEX_RESET]->GetTexturePosition(x, y);
        pimpl_->selectTexture_->SetTexturePosition(x, y);
        pimpl_->selectToolByMouse_ = TOOL_INDEX_RESET;
#ifdef _DEBUG
        std::cout << "Collision is Reset \n";
#endif
    }
    else if (pimpl_->tools_[TOOL_INDEX_START]->CollisionByMouse(x, y) == true) {
        pimpl_->tools_[TOOL_INDEX_START]->GetTexturePosition(x, y);
        pimpl_->selectTexture_->SetTexturePosition(x, y);
        pimpl_->selectToolByMouse_ = TOOL_INDEX_START;
#ifdef _DEBUG
        std::cout << "Collision is Start \n";
#endif
    }
    CMouseController::GetHandle()->SetMouseState(MOUSE_STATE::CLICK_UP);
}

//Draw CTool
void CTool::Draw() {
   
    //Draw Tools
    for (int i = 0; i < pimpl_->tools_.size(); ++i) {
        if (i / 3 == 0) { //Mouse, Wall, Cheese
        pimpl_->tools_[i]->Draw(TOOLS_X_POSITION + i, TOOLS_Y_POSITION);
        }
        else {//Reset, Start Button
            pimpl_->tools_[i]->Draw(TOOLS_X_POSITION+(i%3), TOOLS_Y_POSITION + 1);

        }
    }

    //Draw Select Image 
    pimpl_->selectTexture_->Draw();

}

SharedPtrTexture CTool::GetTexture(TOOL_INDEX index)const {

    switch (index) {
    case TOOL_INDEX_MOUSE:
        return pimpl_->tools_[TOOL_INDEX_MOUSE];
        break;
    case TOOL_INDEX_WALL:
        return pimpl_->tools_[TOOL_INDEX_WALL];
        break;
    case TOOL_INDEX_CHEESE:
        return pimpl_->tools_[TOOL_INDEX_CHEESE];
        break;
    default:
        break;
    }
}
TOOL_INDEX CTool::GetSelectedToolByMouse()const {
    return pimpl_->selectToolByMouse_;
}

void CTool::SetSelectedToolByMouse(TOOL_INDEX index) {
    pimpl_->selectToolByMouse_ = index;
}


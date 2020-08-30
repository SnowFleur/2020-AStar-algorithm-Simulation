#include"MouseController.h"

CMouseController* CMouseController::handle_ = nullptr;

CMouseController::CMouseController() 
    :mouse_x{ 0 }, mouse_y{0 }, mouseState_{ MOUSE_STATE::CLICK_UP } {
}


CMouseController* CMouseController::GetHandle() {

    if (handle_ == nullptr) {
        handle_ = new CMouseController();

    }
    return handle_;
}

void CMouseController::SetMousePosition(const int InputMouseX, const int InputMouseY) {
    mouse_x = InputMouseX;
    mouse_y = InputMouseY;
}

void CMouseController::GetMousePosition(int& refMouseX, int& refMouseY) {
    refMouseX = mouse_x;
    refMouseY = mouse_y;
}

void CMouseController::SetMouseState(MOUSE_STATE state) {
    mouseState_ = state;
}

MOUSE_STATE CMouseController::GetMouseState() {
    return mouseState_;
}

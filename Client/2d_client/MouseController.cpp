#include"MouseController.h"

int CMouseController::mouse_x{ 0 };
int CMouseController::mouse_y{ 0 };
CURRENT_TOOL CMouseController::current_tool_ = CURRENT_TOOL::CURRENT_TOOL_NULL;


void CMouseController::SetControllerByMouse(const int InputMouseX, const int InputMouseY) {
    mouse_x = InputMouseX;
    mouse_y = InputMouseY;
}

void CMouseController::GetControllerByMouse(int& refMouseX, int& refMouseY) {
    refMouseX = mouse_x;
    refMouseY = mouse_y;
}

void CMouseController::SetCurrentTool(CURRENT_TOOL tool) {
    current_tool_ = tool;
}

CURRENT_TOOL CMouseController::GetCurrentTool() {
    return current_tool_;
}

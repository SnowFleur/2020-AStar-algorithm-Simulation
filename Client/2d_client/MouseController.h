#pragma once


enum class CURRENT_TOOL {
    CURRENT_TOOL_NULL   = 0,
    CURRENT_TOOL_MOUSE  = 1,
    CURRENT_TOOL_WALL   = 2,
    CURRENT_TOOL_CHEESE = 3,
    CURRENT_TOOL_RESET  = 4,
};


class CMouseController {
private:
    static int          mouse_x;
    static int          mouse_y;
    static CURRENT_TOOL current_tool_;
public:
    CMouseController() = delete;
    ~CMouseController() = delete;
    CMouseController(const CMouseController&) = delete;
    CMouseController(const CMouseController&&) = delete;
    CMouseController& operator=(const CMouseController&) = delete;
    CMouseController& operator=(const CMouseController&&) = delete;

    static void SetControllerByMouse(const int InputMouseX,const int InputMouseY);
    static void GetControllerByMouse(int& refMouseX, int& refMouseY);
    static void SetCurrentTool(CURRENT_TOOL tool);
    static CURRENT_TOOL GetCurrentTool();

};


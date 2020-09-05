#pragma once
/*
SPDX-FileCopyrightText:     © 2020 DongHyuk Park< SnowFleur0128@gmail.com>
SPDX-License-Identifier:    BSD-3-Clause
More Information visit:     https://snowfleur.tistory.com/159?category=747154
*/
enum class MOUSE_STATE {
    CLICK_UP = 0,
    CLICK_DOWN,
};

/*
singleton Pattern
*/
class CMouseController {
private:
    int                         mouse_x;
    int                         mouse_y;
    MOUSE_STATE                 mouseState_;
    static CMouseController*    handle_;
    CMouseController();
public:
    ~CMouseController() = delete;
    CMouseController(const CMouseController&) = delete;
    CMouseController(CMouseController&&) = delete;
    CMouseController& operator=(const CMouseController&) = delete;
    CMouseController& operator=(CMouseController&&) = delete;

    static CMouseController* GetHandle();
    void SetMousePosition(const int InputMouseX, const int InputMouseY);
    void GetMousePosition(int& refMouseX, int& refMouseY);
    void SetMouseState(MOUSE_STATE state);
    MOUSE_STATE GetMouseState();

};


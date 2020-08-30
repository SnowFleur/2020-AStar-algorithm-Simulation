#pragma once
#include"GameObject.h"

class CTexture;
class Controller;
using BYTE = unsigned char;
using WORD = unsigned short;

enum class ANT_TEXTURE_DIRECTION {
    TEXTURE_UP, TEXTURE_RIGHT, TEXTURE_LEFT, TEXTURE_DOWN
};

/*여왕개미*/
class CQueenAnt :public CGameObject {
private:
    ANT_TEXTURE_DIRECTION   textureDirection_;
    Controller*             controller_;
public:
    CQueenAnt() = delete;
    CQueenAnt(const Position& position, const WORD objectLife, GAME_OBJECT_TYPE objectType, Controller* controller);
    CQueenAnt(Position&& position, const WORD objectLife, GAME_OBJECT_TYPE objectType, Controller* controller);
public://static functions and variable
    static CTexture*        queenAntTexture_;
    static void             SetGameObjectTexture(const wchar_t* fileName, const size_t width,
        const size_t height, const BYTE cycle);
public:
    void                    Update(CWorld&)override;
    void                    Draw()override;
};

/*일개미*/
class CWorkerAnt :public CGameObject {
private:
    ANT_TEXTURE_DIRECTION   textureDirection_;
    Controller*             controller_;
public:
    CWorkerAnt() = delete;
    CWorkerAnt(const Position& position, const WORD objectLife, GAME_OBJECT_TYPE objectType, Controller* controller);
    CWorkerAnt(Position&& position, const WORD objectLife, GAME_OBJECT_TYPE objectType, Controller* controller);
public://static functions and variable
    static CTexture*        workerAntTexture_;
    static void             SetGameObjectTexture(const wchar_t* fileName, const size_t width,
                                                const size_t height, const BYTE cycle);
public:
    void                    Update(CWorld&)override;
    void                    Draw()override;
};

/*수개미*/
class CMaleAnt :public CGameObject {
private:
    ANT_TEXTURE_DIRECTION    textureDirection_;
    Controller* controller_;
public:
    CMaleAnt() = delete;
    CMaleAnt(const Position& position, const WORD objectLife, GAME_OBJECT_TYPE objectType, Controller* controller);
    CMaleAnt(Position&& position, const WORD objectLife, GAME_OBJECT_TYPE objectType, Controller* controller);
public://static functions and variable
    static CTexture*        maleAntTexture_;
    static void             SetGameObjectTexture(const wchar_t* fileName, const size_t width,
        const size_t height, const BYTE cycle);
public:
    void                    Update(CWorld&)override;
    void                    Draw()override;
};




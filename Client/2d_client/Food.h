#pragma once
#include"GameObject.h"

class CTexture;
class Controller;
using BYTE = unsigned char;
using DWORD = unsigned long;
using WORD = unsigned short;

class CFood : public CGameObject {
private:
    static DWORD         spawnTime_;
    Controller*         controller_;
public:
    CFood() = delete;
    CFood(const Position& position, const WORD objectLife, GAME_OBJECT_TYPE objectType, Controller* controller);
    CFood(Position&& position, const WORD objectLife, GAME_OBJECT_TYPE objectType, Controller* controller);
public: //static functions and variable
    static CTexture*    foodtexture_;
    static void         SetGameObjectTexture(const wchar_t* fileName, const size_t width,
                                            const size_t height, const BYTE cycle);
    static void         SetSpawnTime(DWORD time);
    static DWORD         GetSpawnTime();
public:
    void                Update(CWorld& world)override;
    void                Draw()override;
};


#pragma once

using WORD = unsigned short;
class CWorld;

enum class GAME_OBJECT_TYPE {
    QUEEN_ANT = 1, MALE_ANT, WORKER_ANT, FOOD
};



class Position {
private:
public:
   int    x;
   int    y;
};

/*추상클래스*/
class CGameObject  {
private:
    Position                    position_;
    GAME_OBJECT_TYPE            objectType_;
    WORD                        obejectLife_;
    bool                        isUsed_;
    TEXTURE_DIRECTION           textureDirection_;
public:
    CGameObject() = delete;
    virtual ~CGameObject() = default;
    CGameObject(GAME_OBJECT_TYPE Type, Position&& pos, const WORD objectLife);
    CGameObject(GAME_OBJECT_TYPE Type, const Position& pos, const WORD objectLife);
    Position&&                  GetPosition();
    void                        SetPosition(Position&& position);
    void                        SetPosition(const Position& position);
    void                        SetPosition(const WORD x,const WORD y);
    GAME_OBJECT_TYPE            GetGameObjectType()const;
    WORD                        GetObjectLife()const;
    bool                        GetIsUsed()const;
    void                        SetIsUsed(const bool used);
    TEXTURE_DIRECTION           GetTextureDirection()const;
    void                        SetTextureDirection(TEXTURE_DIRECTION texture);
    void                        DecreaseObjectLife();
    void                        SetObjectLife(const WORD life);
public:
    virtual void                Update(CWorld& world) = 0;
    virtual void                Draw() = 0;
};
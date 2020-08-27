#pragma once
#include<memory>


constexpr int TOOLS_X_POSITION = 15;
constexpr int TOOLS_Y_POSITION = 5;

enum TOOL_INDEX {
    TOOL_INDEX_MOUSE = 0,
    TOOL_INDEX_WALL = 1,
    TOOL_INDEX_CHEESE = 2,
    TOOL_INDEX_RESET = 3,
};

class CToolPimpl;
class CTexture;

using SharedPtrToolPimpl = std::shared_ptr<CToolPimpl>;  //using shared ptr Ctool pimpl
using SharedPtrTexture = std::shared_ptr<CTexture>;      //using shared ptr CTexture

class CTool{
private:
    SharedPtrToolPimpl  pimpl_;
public:
    CTool();
    ~CTool() = default;
    CTool(const CTool&) = delete;
    CTool(const CTool&&) = delete;
    CTool& operator=(const CTool&) = delete;
    CTool& operator=(const CTool&&) = delete;

    void Draw();
    void Update();
    SharedPtrTexture GetTexture(TOOL_INDEX index)const;
};


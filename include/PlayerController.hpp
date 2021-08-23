#ifndef PLAYERCONTROLLER_HPP
#define PLAYERCONTROLLER_HPP

#include "TileObject.hpp"

class PlayerController : public TileObject
{
public:
    PlayerController();
    virtual ~PlayerController() = default;
    void Start() override;
    void Update() override;
    void SetTempLogger(TraceLogCallback callback);
private:
    TraceLogCallback callback;
};



#endif
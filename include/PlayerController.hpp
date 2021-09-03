#ifndef PLAYERCONTROLLER_HPP
#define PLAYERCONTROLLER_HPP

#include "TileObject.hpp"

enum Direction {None = -1, Up = 0, Down = 1, Left = 2, Right = 3};

class PlayerController : public TileObject
{
public:
    PlayerController();
    virtual ~PlayerController() = default;
    void Start() override;
    void Update() override;
private:
    float moveStepMultipier;
    bool stoppedMoving;
    Direction inputDirection;
    Direction newInput;
    Direction changeInput;
};



#endif
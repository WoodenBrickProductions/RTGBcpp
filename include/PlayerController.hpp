#ifndef PLAYERCONTROLLER_HPP
#define PLAYERCONTROLLER_HPP

#include "UnitController.hpp"
#include "Data/Blackboard.hpp"

enum Direction {None = -1, Up = 0, Down = 1, Left = 2, Right = 3};

class PlayerController : public UnitController
{
public:
    friend class Player_IdleState;
    friend class Player_MovingState;
    PlayerController();
    virtual ~PlayerController() = default;
    void Start(GameObject* scene, GameState* state) override;
    void Update(GameObject* scene, GameState* state) override;
    void OnDeath(DamageSource damageSource) override;

protected:
    void OnFailedToInitialize() override;
    
private:
    Direction GetInputDirection();
    GridPosition GetTargetPosition(Direction direction);

    Blackboard blackboard;
    bool stoppedMoving;
    Direction inputDirection;
    Direction newInput;
    Direction changeInput;
};



#endif
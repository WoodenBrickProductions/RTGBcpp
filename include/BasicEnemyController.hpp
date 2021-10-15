#ifndef BASICENEMYCONTROLLER_HPP
#define BASICENEMYCONTROLLER_HPP

#include "UnitController.hpp"
#include "Data/AIStats.hpp"
#include "Data/Blackboard.hpp"

class BasicEnemyController : public UnitController
{
protected:
    friend class BE_IdleState;
    friend class BE_MovingState;
    friend class BE_ChasingState;
    Blackboard blackboard;

    AIStats aiStats;
    std::vector<GridPosition> possibleMoves;
    bool stoppedMoving;
    GridPosition lastPlayerPosition;

    static bool IsObjectWithinRange(TileObject* object1, TileObject* object2, int range);
    std::vector<GridPosition> FindPathToObject(TileObject* object);
    State* Wander();
    State* Chase(TileObject* object);
    // void ChangeState(State* newState) override;

public:
    BasicEnemyController();
    virtual ~BasicEnemyController() = default;
    void Start(GameObject* scene, GameState* gameState) override;
    void Update(GameObject* scene, GameState* gameState) override;
    void OnTargetObjectKilled(IAttackable* target) override;
};
#endif
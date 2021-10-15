#ifndef UNITSTATUS_HPP
#define UNITSTATUS_HPP

class UnitStatus
{
public:
    bool movable = true;
    bool attackable = true;
    bool effectable = true;
    bool interactable = true;
    bool canAttack = true;

    UnitStatus(){}

    virtual ~UnitStatus() = default;

    UnitStatus(UnitStatus& unitStatus)
    {
        movable = unitStatus.movable;
        attackable = unitStatus.attackable;
        effectable = unitStatus.effectable;
        interactable = unitStatus.interactable;
        canAttack = unitStatus.canAttack;
    }
};

#endif
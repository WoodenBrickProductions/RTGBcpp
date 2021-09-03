#ifndef UNITSTATUS_HPP
#define UNITSTATUS_HPP

class UnitStatus
{
public:
    UnitStatus()
    {
        movable = true;
        attackable = true;
        effectable = true;
        interactable = true;
        canAttack = true;
    }

    virtual ~UnitStatus() = default;

    UnitStatus(UnitStatus& unitStatus)
    {
        movable = unitStatus.movable;
        attackable = unitStatus.attackable;
        effectable = unitStatus.effectable;
        interactable = unitStatus.interactable;
        canAttack = unitStatus.canAttack;
    }

    bool movable;
    bool attackable;
    bool effectable;
    bool interactable;
    bool canAttack;

};

#endif
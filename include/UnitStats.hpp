#ifndef UNITSTATS_HPP
#define UNITSTATS_HPP

class UnitStats 
{
public:
    int maxHealth = 1;
    int currentHealth = 1;
    int attackDamage = 1;
    float attackSpeed = 1;
    int attackRange = 1;
    float attackTime = 0;
    int experience = 1;
    int level = 1;
    float movementSpeed = 1;
    

    UnitStats(){}

    virtual ~UnitStats() = default;

    UnitStats(UnitStats& unitStats)
    {
        maxHealth = unitStats.maxHealth;
        currentHealth = unitStats.currentHealth;
        attackDamage = unitStats.attackDamage;
        experience = unitStats.experience;
        level = unitStats.level;
        attackRange = unitStats.attackRange;
        attackTime = unitStats.attackTime;
        attackSpeed = unitStats.attackSpeed;
        movementSpeed = unitStats.movementSpeed;
    }
};

#endif
#ifndef UNITSTATS_HPP
#define UNITSTATS_HPP

class UnitStats 
{
public:
    UnitStats()
    {
        maxHealth = 1;
        currentHealth = 1;
        attackDamage = 1;
        attackSpeed = 1;
        attackRange = 1;
        experience = 1;
        level = 1;
        movementSpeed = 1;
    }

    virtual ~UnitStats() = default;

    UnitStats(UnitStats& unitStats)
    {
        maxHealth = unitStats.maxHealth;
        currentHealth = unitStats.currentHealth;
        attackDamage = unitStats.attackDamage;
        experience = unitStats.experience;
        level = unitStats.level;
        attackRange = unitStats.attackRange;
        attackSpeed = unitStats.attackSpeed;
        movementSpeed = unitStats.movementSpeed;
    }

    int maxHealth;
    int currentHealth;
    int attackDamage;
    float attackSpeed;
    int attackRange;
    int experience;
    int level;
    float movementSpeed;
};

#endif
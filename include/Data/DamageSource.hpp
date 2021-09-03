#ifndef DAMAGESOURCE_HPP
#define DAMAGESOURCE_HPP

typedef enum {
    Attack,
    Effect
} DamageType;

class IDealsDamage;
class DamageSource
{
public:
    DamageSource(IDealsDamage* source, DamageType type, int damage)
    {
        damageSource = source;
        damageType = type;
        damageAmount = damage;
    }

    virtual ~DamageSource() = default;

    IDealsDamage* damageSource;
    DamageType damageType;
    int damageAmount;

};

#endif
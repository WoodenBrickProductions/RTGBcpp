#ifndef DAMAGESOURCE_HPP
#define DAMAGESOURCE_HPP

enum DamageType {
    DAMAGE_TYPE_ATTACK,
    DAMAGE_TYPE_EFFECT
};

class IDealsDamage;
class DamageSource
{
public:
    DamageSource(IDealsDamage* source, DamageType type, int damage)
    {
        this->source = source;
        damageType = type;
        damageAmount = damage;
    }

    virtual ~DamageSource() = default;

    IDealsDamage* source;
    DamageType damageType;
    int damageAmount;

};

#endif
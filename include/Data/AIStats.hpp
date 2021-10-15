#ifndef AISTATS_HPP
#define AISTATS_HPP

class AIStats
{
public:
    float wanderCooldown = 1;
    float wanderTime = 0;
    
    int agroRange = 1;
    int talkRange = 1;
    
    bool usePseudoRandom = false;
    
    float chasingCooldown = 0.25f;
    float chasingTime = 0;
    int chasingRange = 100;
    bool chasing = false;

    AIStats(){}

    virtual ~AIStats() = default;

    AIStats(AIStats& aiStats)
    {
        wanderCooldown = aiStats.wanderCooldown;
        agroRange = aiStats.agroRange;
        chasingRange = aiStats.chasingRange;
        chasingCooldown = aiStats.chasingCooldown;
        usePseudoRandom = aiStats.usePseudoRandom;
        wanderTime = aiStats.wanderTime;
        chasingTime = aiStats.chasingTime;
        chasing = aiStats.chasing;
    }
};

#endif
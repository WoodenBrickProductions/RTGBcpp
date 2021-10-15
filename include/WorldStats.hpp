#ifndef WORLDSTATS_HPP
#define WORLDSTATS_HPP

class WorldStats
{
public:
    bool worldSpacing = 1;
    
    WorldStats(){}

    virtual ~WorldStats() = default;

    WorldStats(WorldStats& worldStats)
    {
        worldSpacing = worldStats.worldSpacing;
    }
};

#endif
#ifndef WORLDSTATS_HPP
#define WORLDSTATS_HPP

class WorldStats
{
public:
    WorldStats()
    {
        worldSpacing = 1;
    }

    virtual ~WorldStats() = default;

    WorldStats(WorldStats& worldStats)
    {
        worldSpacing = worldStats.worldSpacing;
    }

    bool worldSpacing;

};

#endif
#ifndef BLACKBOARD_HPP
#define BLACKBOARD_HPP
#include <map>
#include <string>
#include <stdlib.h>
#include "CustomLogger.hpp"

class Blackboard
{
public:
    Blackboard() {};
    virtual ~Blackboard() = default;
    void* Get(std::string index)
    {
        it = blackboard.find(index);
        if(it != blackboard.end())
        {
            printf("%s", it->first.c_str());
            printf("%p", it->second);
            LogCustom(0, "Found key, returning", nullptr);
            return it->second;
        }

        std::string out = "Couldn't find key with index: " + index;
        LogCustom(3, out.c_str(), nullptr);
        return nullptr;
    };

    void Insert(std::string index, void* value)
    {
        std::pair<std::map<std::string,void*>::iterator,bool> ret;
        ret = blackboard.insert(std::pair<std::string, void*>(index, value));
        if(ret.second == false)
        {
            blackboard[index] = value;
        }
    }

private:
    std::map<std::string, void*> blackboard;
    std::map<std::string, void*>::iterator it;

};

#endif
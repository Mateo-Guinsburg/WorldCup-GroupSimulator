#pragma once
#include"group.h"
#include<vector>
#include<memory>

class Tournament{
    private:
        std::vector<Group> groups;
    public:
        Tournament(){}
        void addGroup(const Group &group);
        void simulateGroupStage();
        std::vector<std::shared_ptr<Team>> getRoundOf32();
        void loadTeamsFromFile(const std::string& filename);
};
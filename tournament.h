#pragma once
#include"group.h"

class Tournament{
    private:
        std::vector<Group> groups;
        int searchGroup(const std::string& groupName) const;
    public:
        Tournament(){}
        void addGroup(const Group &group);
        void simulateGroupStage();
        void loadTeamsFromFile(const std::string& filename);
        
        // Display methods
        void printGroupStandings(const std::string& groupName);
        void printThirdPlaceStandings();
    
        // Interactive simulator
        void startInteractiveSimulator();

        void registerMatchResult(const std::string& groupName, const std::string& t1Name, const std::string& t2Name, int s1, int s2);
};
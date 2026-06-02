#pragma once
#include"group.h"
#include<algorithm>
#include<fstream>
#include<sstream>
#include<iostream>
#include<vector>
#include<memory>
#include<map>
#include<string>
#include<iomanip> 
#include<iostream>

class Tournament{
    private:
        std::vector<Group> groups;
        int searchGroup(const std::string& groupName) const;
        void addMatchToMemory(const std::string& groupName, const std::string& t1Name, const std::string& t2Name, int s1, int s2);
    public:
        Tournament(){}
        void addGroup(const Group &group);

        void loadTeamsFromFile(const std::string& filename);
        void saveMatchResultToFile(const std::string& groupName, const std::string& t1Name, const std::string& t2Name, int s1, int s2);
        void loadExistingResults(const std::string& filename);
        void registerMatchResult(const std::string& groupName, const std::string& t1Name, const std::string& t2Name, int s1, int s2);


        // HTML Generators for the Web Interface
        std::string getGroupStandingsHTML(const std::string& groupName);
        std::string getThirdPlaceStandingsHTML();
};
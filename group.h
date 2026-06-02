#pragma once
#include "team.h"
#include"match.h"
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <stdexcept>

class Group{
private:
    std::string groupName;
    std::vector<std::shared_ptr<Team>> teams;
    std::vector<std::shared_ptr<Match>> matches;

public:
    //Constructor
    Group(const std::string& name):groupName(name){}

    void addTeam(const std::shared_ptr<Team>& team);
    void addMatch(const std::shared_ptr<Match>& match);

    //Uses FIFA rules to sort the group
    void sortStandings();
    
    std::vector<std::shared_ptr<Team>> getTopTwo() const;
    std::shared_ptr<Team> getThirdPlace() const;
    std::string getName() const {return groupName;}
    
    //Getter to print table
    const std::vector<std::shared_ptr<Team>>& getTeams() const {return teams;}
    
};
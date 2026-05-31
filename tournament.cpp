#include"tournament.h"
#include<algorithm>
#include<fstream>
#include<sstream>
#include<iostream>

void Tournament::addGroup(const Group &group){
    groups.push_back(group);
}

void Tournament::simulateGroupStage(){
    for(auto &g : groups){
        g.sortStandings();
    }
}

std::vector<std::shared_ptr<Team>> Tournament::getRoundOf32(){
    std::vector<std::shared_ptr<Team>> qualifiedTeams;
    std::vector<std::shared_ptr<Team>> allThirdPlaces;
    for(auto &g : groups){
        auto topTwo = g.getTopTwo();
        qualifiedTeams.insert(qualifiedTeams.end(), topTwo.begin(), topTwo.end());
        allThirdPlaces.push_back(g.getThirdPlace());
    }

    //Lambda function with FIFA rules for third place
    auto fifaRules = [](const std::shared_ptr<Team>& a, const std::shared_ptr<Team>& b){
        if (a->getPoints() != b->getPoints()){
            return a->getPoints() > b->getPoints(); 
        }

        if (a->getGoalDifference() != b->getGoalDifference()){
            return a->getGoalDifference() > b->getGoalDifference();
        }

        if (a->getGoalsFor() != b->getGoalsFor()){
            return a->getGoalsFor() > b->getGoalsFor();
        }

        if (a->getFairPlay() != b->getFairPlay()){
            return a->getFairPlay() > b->getFairPlay();
        }

        return a->getFifaRanking() < b->getFifaRanking();
    };

    std::sort(allThirdPlaces.begin(), allThirdPlaces.end(), fifaRules);

    for(int i = 0; i < 8; i++){
        qualifiedTeams.push_back(allThirdPlaces[i]);
    }

    return qualifiedTeams;
}

void Tournament::loadTeamsFromFile(const std::string& filename){
    std::ifstream file(filename);
    if (!file.is_open()){
        throw std::runtime_error("Error: Could not open the teams file.");
    }

    std::string line, name, groupName, rankingStr;
    
    while (std::getline(file, line)){
        std::istringstream iss(line);
        if (std::getline(iss, name, ',') && std::getline(iss, groupName, ',') && std::getline(iss, rankingStr)){
            try {
                int ranking = std::stoi(rankingStr); //Ranking conversion to an int
                auto newTeam = std::make_shared<Team>(name, groupName, ranking);

                // Search for the group
                auto it = std::find_if(groups.begin(), groups.end(), [&](const Group& g) {return g.getName() == groupName;});
                
                if (it != groups.end()) {
                    it->addTeam(newTeam);
                } else {
                    Group newGroup(groupName);
                    newGroup.addTeam(newTeam);
                    groups.push_back(newGroup);
                }
            } catch (...) {
                std::cout << "Warning: Error parsing line: " << line << "\n";
            }
        }
    }
    file.close();
}
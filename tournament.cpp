#include"tournament.h"
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


//Returns group iterator in groups vector
int Tournament::searchGroup(const std::string& groupName) const{
    bool groupFound = false;
    int it = 0;
    for(int i = 0; i < groups.size(); i++){
        if(groups[i].getName() == groupName){
            it = i;
            groupFound = true;
            break;
        }
    }

    if(!groupFound){
        throw std::runtime_error("Error: Could not found group.");
    }

    return it;
}

void Tournament::addGroup(const Group &group){
    groups.push_back(group);
}

void Tournament::simulateGroupStage(){
    for(auto &g : groups){
        g.sortStandings();
    }
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

                //Search for the group
                bool groupFound = false;
                
                for(int i = 0; i < groups.size(); i++){
                    if(groupName == groups[i].getName()){
                        groups[i].addTeam(newTeam);
                        groupFound = true;
                        break;
                    }
                }

                if(!groupFound){
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

//Prints a specific group table
void Tournament::printGroupStandings(const std::string& groupName) {
    int it = searchGroup(groupName); 
    
    groups[it].sortStandings();

    std::cout << "\n=============== GROUP " << groups[it].getName() << " ================\n";
    std::cout << std::left << std::setw(15) << "Team" 
            << std::right << std::setw(5) << "Pts" 
            << std::setw(6) << "P " //Matches played
            << std::setw(5) << "GF" //Goals scored
            << std::setw(5) << "GD" //Goal difference
            << std::setw(5) << "FP" << "\n"; //Fair Play
    std::cout << "----------------------------------------\n";

    for (const auto& team : groups[it].getTeams()) {
        std::cout << std::left << std::setw(15) << team->getName()
                << std::right << std::setw(5) << team->getPoints()
                << std::setw(5) << team->getMatchesPlayed()
                << std::setw(5) << team->getGoalsFor()
                << std::setw(5) << std::showpos << team->getGoalDifference() << std::noshowpos
                << std::setw(5) << team->getFairPlay() << "\n";
        }
    std::cout << "========================================\n";
}


void Tournament::printThirdPlaceStandings() {
    std::vector<std::shared_ptr<Team>> allThirdPlaces;
    for(auto &g : groups){
        g.sortStandings();
        auto third = g.getThirdPlace();
        if (third != nullptr) {
            allThirdPlaces.push_back(third);
        }
    }

    //FIFA rules for third places
    auto fifaRules = [](const std::shared_ptr<Team>& a, const std::shared_ptr<Team>& b){
        //Points
        if(a->getPoints() != b->getPoints()){
            return a->getPoints() > b->getPoints();
        }
        //Goal difference
        if(a->getGoalDifference() != b->getGoalDifference()){
            return a->getGoalDifference() > b->getGoalDifference();
        }
        //Goals scored
        if(a->getGoalsFor() != b->getGoalsFor()){
            return a->getGoalsFor() > b->getGoalsFor();
        }
        //Fair Play
        if(a->getFairPlay() != b->getFairPlay()){
            return a->getFairPlay() > b->getFairPlay();
        } 
        //Fifa ranking
        return a->getFifaRanking() < b->getFifaRanking();
    };

    std::sort(allThirdPlaces.begin(), allThirdPlaces.end(), fifaRules);

    std::cout << "\n=========== BEST THIRD PLACES ===========\n";
    std::cout << " (Top 8 qualify for the Round of 32)\n";
    std::cout << "-----------------------------------------\n";
    
    for(size_t i = 0; i < allThirdPlaces.size(); i++) {
        if (i == 8) std::cout << "--- CUT-OFF LINE (ELIMINATED) ---\n";
        
        auto team = allThirdPlaces[i];
        std::cout << i + 1 << ". " << std::left << std::setw(15) << team->getName()
                  << "[Grp " << team->getGroup() << "] " 
                  << std::right << std::setw(3) << team->getPoints() << " pts | "
                  << "GD: " << std::showpos << std::setw(2) << team->getGoalDifference() << std::noshowpos << " | "
                  << "GF: " << team->getGoalsFor() << "\n";
    }
    std::cout << "=========================================\n";
}

//Sets a match result into the correct group
void Tournament::registerMatchResult(const std::string& groupName, const std::string& t1Name, const std::string& t2Name, int s1, int s2) {
    try {
        int gIt = searchGroup(groupName);
            
        std::shared_ptr<Team> team1 = nullptr;
        std::shared_ptr<Team> team2 = nullptr;
            
        const auto& teams = groups[gIt].getTeams();
        for(size_t i = 0; i < teams.size(); i++){
            if(teams[i]->getName() == t1Name) team1 = teams[i];
            if(teams[i]->getName() == t2Name) team2 = teams[i];
        }
            
        //If both teams exist play the match
        if(team1 != nullptr && team2 != nullptr){
            auto match = std::make_shared<Match>(team1, team2);
            match->playMatch(s1, s2);
            groups[gIt].addMatch(match);
                
            std::cout << "Match registered: " << t1Name << " " << s1 << " - " << s2 << " " << t2Name << "\n";
        }else{
            std::cout << "ERROR: Could not find one or both teams in Group " << groupName << ".\n";
        }
            
    }catch(const std::exception& e){
        std::cout << e.what() << "\n";
    }
}

// 4. The main loop for the user to interact with the simulator
void Tournament::startInteractiveSimulator() {
    int choice = 0;
    while (choice != 4) {
        std::cout << "\n=== WORLD CUP 2026 - SCENARIO SIMULATOR ===\n";
        std::cout << "1. View Group Standings\n";
        std::cout << "2. View Third-Place Qualification Table\n";
        std::cout << "3. Add Match Result\n";
        std::cout << "4. Exit\n";
        std::cout << "Select an option: ";
        std::cin >> choice;

        if (choice == 1) {
            std::string gName;
            std::cout << "Enter Group Letter (A-L): ";
            std::cin >> gName;
            printGroupStandings(gName);
        } 
        else if (choice == 2) {
            printThirdPlaceStandings();
        }
        else if (choice == 3) {
            std::string gName, t1, t2;
            int s1, s2;
            std::cout << "Enter Group (A-L): ";
            std::cin >> gName;
            std::cout << "Enter Team 1 Name: ";
            std::cin >> t1;
            std::cout << "Enter Team 1 Goals: ";
            std::cin >> s1;
            std::cout << "Enter Team 2 Name: ";
            std::cin >> t2;
            std::cout << "Enter Team 2 Goals: ";
            std::cin >> s2;
            
            registerMatchResult(gName, t1, t2, s1, s2);
        }
    }
}
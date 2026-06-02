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

void Tournament::addMatchToMemory(const std::string& groupName, const std::string& t1Name, const std::string& t2Name, int s1, int s2) {
    int gIt = searchGroup(groupName);
    
    std::shared_ptr<Team> team1 = nullptr;
    std::shared_ptr<Team> team2 = nullptr;
    
    const auto& teams = groups[gIt].getTeams();
    for(size_t i = 0; i < teams.size(); i++) {
        if(teams[i]->getName() == t1Name) team1 = teams[i];
        if(teams[i]->getName() == t2Name) team2 = teams[i];
    }
    
    if(team1 != nullptr && team2 != nullptr) {
        auto match = std::make_shared<Match>(team1, team2);
        match->playMatch(s1, s2);
        groups[gIt].addMatch(match);
    } else {
        throw std::runtime_error("Could not find teams in group.");
    }
}

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

void Tournament::loadTeamsFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Error: Could not open the teams file.");
    }

    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        if (line.empty()) continue;

        std::istringstream iss(line);
        std::string name, groupName, rankingStr;
        
        if (std::getline(iss, name, ',') && std::getline(iss, groupName, ',') && std::getline(iss, rankingStr)) {
            try {
                rankingStr.erase(std::remove_if(rankingStr.begin(), rankingStr.end(), ::isspace), rankingStr.end());
                
                int ranking = std::stoi(rankingStr);
                auto newTeam = std::make_shared<Team>(name, groupName, ranking);

                try {
                    int gIt = searchGroup(groupName);
                    groups[gIt].addTeam(newTeam);
                } catch (...) {
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

//Save results in results.csv
void Tournament::saveMatchResultToFile(const std::string& groupName, const std::string& t1Name, const std::string& t2Name, int s1, int s2) {
    std::ofstream file("results.csv", std::ios::app);
    if (file.is_open()) {
        file << groupName << "," << t1Name << "," << t2Name << "," << s1 << "," << s2 << "\n";
        file.close();
    }
}

//Load results
void Tournament::loadExistingResults(const std::string& filename){
    std::ifstream file(filename);
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string g, t1, t2, s1, s2;
        if(std::getline(ss, g, ',') && std::getline(ss, t1, ',') && std::getline(ss, t2, ',') && std::getline(ss, s1, ',') && std::getline(ss, s2)){
            addMatchToMemory(g, t1, t2, std::stoi(s1), std::stoi(s2)); 
        }
    }
}


//Prints a specific group table
void Tournament::printGroupStandings(const std::string& groupName){
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
        addMatchToMemory(groupName, t1Name, t2Name, s1, s2);
        saveMatchResultToFile(groupName, t1Name, t2Name, s1, s2); 
        std::cout << "Match registered: " << t1Name << " " << s1 << " - " << s2 << " " << t2Name << "\n";
    } catch(const std::exception& e) {
        std::cout << "ERROR: " << e.what() << "\n";
    }
}

//The main loop for the user to interact with the simulator
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


// Generates an HTML table for a specific group
std::string Tournament::getGroupStandingsHTML(const std::string& groupName) {
    try {
        int it = searchGroup(groupName);
        groups[it].sortStandings();

        std::stringstream html;
        html << "<h2 style='color: #1a73e8;'>Group " << groups[it].getName() << "</h2>";
        html << "<table class='standings-table'>";
        html << "<tr><th>Team</th><th>Pts</th><th>MP</th><th>GF</th><th>GD</th><th>FP</th></tr>";

        for (const auto& team : groups[it].getTeams()) {
            html << "<tr>"
                 << "<td>" << team->getName() << "</td>"
                 << "<td><strong>" << team->getPoints() << "</strong></td>"
                 << "<td>" << team->getMatchesPlayed() << "</td>"
                 << "<td>" << team->getGoalsFor() << "</td>"
                 << "<td>";
                 if (team->getGoalDifference() > 0) html << "+"; // Visual plus sign
                 html << team->getGoalDifference() << "</td>"
                 << "<td>" << team->getFairPlay() << "</td>"
                 << "</tr>";
        }
        html << "</table>";
        return html.str();
        
    } catch (const std::exception& e) {
        return std::string("<div class='error-msg'>") + e.what() + "</div>";
    }
}

// Generates an HTML table for the third places
std::string Tournament::getThirdPlaceStandingsHTML() {
    std::vector<std::shared_ptr<Team>> allThirdPlaces;
    for(auto &g : groups){
        g.sortStandings();
        auto third = g.getThirdPlace();
        if (third != nullptr) {
            allThirdPlaces.push_back(third);
        }
    }

    auto fifaRules = [](const std::shared_ptr<Team>& a, const std::shared_ptr<Team>& b){
        if(a->getPoints() != b->getPoints()) return a->getPoints() > b->getPoints();
        if(a->getGoalDifference() != b->getGoalDifference()) return a->getGoalDifference() > b->getGoalDifference();
        if(a->getGoalsFor() != b->getGoalsFor()) return a->getGoalsFor() > b->getGoalsFor();
        if(a->getFairPlay() != b->getFairPlay()) return a->getFairPlay() > b->getFairPlay();
        return a->getFifaRanking() < b->getFifaRanking();
    };

    std::sort(allThirdPlaces.begin(), allThirdPlaces.end(), fifaRules);

    std::stringstream html;
    html << "<h2 style='color: #1a73e8;'>Best Third Places</h2>";
    html << "<table class='standings-table'>";
    html << "<tr><th>#</th><th>Team</th><th>Group</th><th>Pts</th><th>GD</th><th>GF</th></tr>";

    for(size_t i = 0; i < allThirdPlaces.size(); i++) {
        auto team = allThirdPlaces[i];
        
        // Dynamic CSS classes depending on qualification
        std::string rowClass = (i < 8) ? "qualified" : "eliminated";
        
        if (i == 8) {
            html << "<tr><td colspan='6' class='divider'>--- ELIMINATION CUT-OFF ---</td></tr>";
        }

        html << "<tr class='" << rowClass << "'>"
             << "<td>" << (i + 1) << "</td>"
             << "<td>" << team->getName() << "</td>"
             << "<td>" << team->getGroup() << "</td>"
             << "<td><strong>" << team->getPoints() << "</strong></td>"
             << "<td>";
             if (team->getGoalDifference() > 0) html << "+";
             html << team->getGoalDifference() << "</td>"
             << "<td>" << team->getGoalsFor() << "</td>"
             << "</tr>";
    }
    html << "</table>";
    return html.str();
}
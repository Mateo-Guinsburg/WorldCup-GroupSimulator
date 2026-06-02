#include "group.h"

void Group::addTeam(const std::shared_ptr<Team>& team) {
    if (teams.size() >= 4) {
        throw std::invalid_argument("Error: A World Cup group cannot have more than 4 teams.");
    }
    teams.push_back(team);
}

void Group::addMatch(const std::shared_ptr<Match>& match){
    matches.push_back(match);
    sortStandings();
}

void Group::sortStandings() {
    //Lambda function with FIFA rules
    auto fifaRules = [this](const std::shared_ptr<Team>& a, const std::shared_ptr<Team>& b) {
        //First criteria: points
        if (a->getPoints() != b->getPoints()) {
            return a->getPoints() > b->getPoints(); 
        }

        //Second criteria: goal difference in the match between them
        for(const auto& match : this->matches){
           if(match->hasTeams(a, b)){
                auto winner = match->getWinner();
                if(winner != nullptr){
                    return winner == a;
                }
                break;
           }
        }

        //Third criteria: global goal difference
        if (a->getGoalDifference() != b->getGoalDifference()) {
            return a->getGoalDifference() > b->getGoalDifference();
        }

        //Fourth criteria: global goalsFor
        if (a->getGoalsFor() != b->getGoalsFor()){
            return a->getGoalsFor() > b->getGoalsFor();
        }

        //Fifth criteria: fair play
        if (a->getFairPlay() != b->getFairPlay()) {
            return a->getFairPlay() > b->getFairPlay();
        }

        //Sixth criteria: fifa ranking
        return a->getFifaRanking() < b->getFifaRanking();
    };

    std::sort(teams.begin(), teams.end(), fifaRules);
}

std::vector<std::shared_ptr<Team>> Group::getTopTwo() const{
    return {teams[0], teams[1]};
}

std::shared_ptr<Team> Group::getThirdPlace() const{
    if (teams.size() < 3) {
        return nullptr;
    }
    return teams[2]; 
}
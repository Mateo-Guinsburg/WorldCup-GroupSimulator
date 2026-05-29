#include"team.h"

std::string Team::getName() const{return name;}
int Team::getPoints() const{return points;}
int Team::getGoalDifference() const{return goalsFor - goalsAgainst;}
int Team::getGoalsFor() const{return goalsFor;}
int Team::getMatchesPlayed() const{return matchesPlayed;}
int Team::getFairPlay() const {return fairPlayPoints; }
int Team::getFifaRanking() const {return fifaRanking;}

void Team::addResult(int scored, int conceded){
    goalsFor += scored;
    goalsAgainst += conceded;
    matchesPlayed++;
    if (scored > conceded) points += 3;       
    else if (scored == conceded) points += 1; 
}

void Team::applyCards(int yellow, int red){
    fairPlayPoints -= (yellow * 1) + (red * 3);
}
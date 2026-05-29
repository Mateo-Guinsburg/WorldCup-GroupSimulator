#pragma once
#include <string>

class Team{
private:
    std::string name;
    std::string group;
    int points;
    int goalsFor;
    int goalsAgainst;
    int matchesPlayed;
    int fairPlayPoints;
    int fifaRanking;

public:
    Team(std::string name, std::string group, int ranking):name(name), group(group), points(0), goalsFor(0), 
    goalsAgainst(0), matchesPlayed(0), fairPlayPoints(0), fifaRanking(ranking){}

    // Getters
    std::string getName() const;
    int getPoints() const;
    int getGoalDifference() const;
    int getGoalsFor() const;
    int getMatchesPlayed() const;
    int getFairPlay() const;
    int getFifaRanking() const;

    // Method to update team stats after a match
    void addResult(int scored, int conceded);
    void applyCards(int yellow, int red);
};
#pragma once
#include"team.h"
#include<memory>

class Match{
    private:
        std::shared_ptr<Team> team1;
        std::shared_ptr<Team> team2;
        int score1;
        int score2;
        bool isPlayed;

    public:
        Match(const std::shared_ptr<Team> &t1, const std::shared_ptr<Team> &t2): team1(t1), team2(t2), score1(0), score2(0), isPlayed(false){}
        void playMatch(int s1, int s2);
        std::shared_ptr<Team> getWinner() const;
        bool hasTeams(const std::shared_ptr<Team>& a, const std::shared_ptr<Team>& b) const;
};
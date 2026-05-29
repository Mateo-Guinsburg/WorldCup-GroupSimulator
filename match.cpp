#include"match.h"

void Match::playMatch(int s1, int s2){
    score1 = s1;
    score2 = s2;
    isPlayed = true;
    team1->addResult(score1, score2);
    team2->addResult(score2, score1);
}

std::shared_ptr<Team> Match::getWinner() const{
    if (!isPlayed) return nullptr;
    if (score1 > score2) return team1;
    if (score2 > score1) return team2;
    return nullptr; //In case of draw   
}

bool Match::hasTeams(const std::shared_ptr<Team>& a, const std::shared_ptr<Team>& b) const{
    return (team1 == a && team2 == b) || (team1 == b && team2 == a);
}
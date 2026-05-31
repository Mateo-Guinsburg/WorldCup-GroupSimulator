#include<iostream>
#include"tournament.h"

int main() {
    Tournament worldCup;
    try {
        worldCup.loadTeamsFromFile("teams.csv");
        std::cout << "Teams loaded successfully!\n";
    } catch (const std::exception& e) {
        std::cerr << e.what() << "\n";
    }
}
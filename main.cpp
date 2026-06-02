#include <iostream>
#include "tournament.h"

int main() {
    Tournament worldCup;
    
    try {
        worldCup.loadTeamsFromFile("teams.csv");
        std::cout << "Teams loaded successfully! Simulator ready.\n";
    } catch (const std::exception& e) {
        std::cerr << e.what() << "\n";
        return 1;
    }

    //Launch the interactive tracker
    worldCup.startInteractiveSimulator();

    return 0;
}
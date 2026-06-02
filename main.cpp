#include <iostream>
#include "tournament.h"
#include "httplib.h"

int main() {
    Tournament worldCup;
    
    try {
        worldCup.loadTeamsFromFile("teams.csv");
        worldCup.loadExistingResults("results.csv");
        std::cout << "Data loaded successfully!\n";
    } catch (const std::exception& e) {
        std::cerr << e.what() << "\n";
        return 1;
    }

    httplib::Server svr;

    svr.set_mount_point("/", "./");
    svr.Get("/", [](const httplib::Request& req, httplib::Response& res) {
        res.set_redirect("/index.html");
    });

    
    svr.Get("/standings", [&](const httplib::Request& req, httplib::Response& res) {
        std::string groupLetter = req.get_param_value("group");
        std::string htmlResponse = worldCup.getGroupStandingsHTML(groupLetter);
        res.set_content(htmlResponse, "text/html");
    });

    
    svr.Get("/third-places", [&](const httplib::Request& req, httplib::Response& res) {
        std::string htmlResponse = worldCup.getThirdPlaceStandingsHTML();
        res.set_content(htmlResponse, "text/html");
    });

    svr.Get("/add-match", [&](const httplib::Request& req, httplib::Response& res) {
        std::string g = req.get_param_value("group");
        std::string t1 = req.get_param_value("t1");
        std::string t2 = req.get_param_value("t2");
        int g1 = std::stoi(req.get_param_value("g1"));
        int g2 = std::stoi(req.get_param_value("g2"));

        worldCup.registerMatchResult(g, t1, t2, g1, g2);
        
        std::string successHTML = "<div class='success-msg'>Match Registered Successfully!<br>" + t1 + " " + std::to_string(g1) + " - " + std::to_string(g2) + " " + t2 + "</div>";
        res.set_content(successHTML, "text/html");
    });

    std::cout << "=================================================\n";
    std::cout << " SERVER RUNNING AT: http://localhost:8080\n";
    std::cout << " Open that link in your browser.\n";
    std::cout << " Press Ctrl+C in this console to stop the server.\n";
    std::cout << "=================================================\n";
    
    svr.listen("localhost", 8080);

    return 0;
}
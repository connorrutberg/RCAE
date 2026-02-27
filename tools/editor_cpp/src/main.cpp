#include "rcae/EditorApp.hpp"

#include <iostream>
#include <string>

int main() {
    rcae::EditorApp app;
    try {
        app.loadDefaultScripts();
    } catch (const std::exception& e) {
        std::cerr << "startup error: " << e.what() << "\n";
        return 1;
    }

    std::cout << "RCAE C++ Editor Prototype - commands: entities, select <id>, inspect, scripts [query], addscript <Name>, set <Script> <Prop> <Value>, import <path>, export <path>, quit\n";

    std::string line;
    while (true) {
        std::cout << "rcae> ";
        if (!std::getline(std::cin, line)) break;
        if (line == "quit") break;
        try {
            std::cout << app.handle(line) << "\n";
        } catch (const std::exception& e) {
            std::cout << "error: " << e.what() << "\n";
        }
    }

    return 0;
}

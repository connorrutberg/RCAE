#include "rcae/runtime/GameRuntimeApp.hpp"

#include <iostream>
#include <string>

int main(int argc, char** argv) {
    const std::string levelPath = (argc > 1) ? argv[1] : "games/game_alpha/content/levels/level_01.map";

    rcae::runtime::GameRuntimeApp app;
    std::string err;
    if (!app.loadLevelFromAsciiFile(levelPath, err)) {
        std::cerr << "load error: " << err << "\n";
        return 1;
    }

    std::cout << "RCAE Game App\n";
    std::cout << "Commands: w/a/s/d, stop, tick, run <n>, show, quit\n\n";
    std::cout << app.frame() << "\n";

    std::string cmd;
    while (true) {
        std::cout << "game> ";
        if (!std::getline(std::cin, cmd)) break;
        if (cmd == "quit") break;

        if (cmd == "w") app.setPlayerVelocity(0, -1);
        else if (cmd == "a") app.setPlayerVelocity(-1, 0);
        else if (cmd == "s") app.setPlayerVelocity(0, 1);
        else if (cmd == "d") app.setPlayerVelocity(1, 0);
        else if (cmd == "stop") app.setPlayerVelocity(0, 0);
        else if (cmd.rfind("run ", 0) == 0) {
            int n = std::stoi(cmd.substr(4));
            for (int i = 0; i < n; ++i) app.tick();
            std::cout << app.frame() << "\n";
            const auto events = app.consumeEventLog();
            if (!events.empty()) std::cout << events;
            continue;
        } else if (cmd == "show") {
            std::cout << app.frame() << "\n";
            continue;
        } else if (cmd != "tick") {
            std::cout << "unknown command\n";
            continue;
        }

        app.tick();
        std::cout << app.frame() << "\n";
        const auto events = app.consumeEventLog();
        if (!events.empty()) std::cout << events;
    }

    return 0;
}

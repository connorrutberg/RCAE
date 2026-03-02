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
    std::cout << "Commands: w/a/s/d, stop, tick, run <n>, click <button_id>, show, quit\n\n";
    std::cout << "Commands: w/a/s/d, stop, tick, run <n>, show, quit\n\n";
    std::cout << app.frame() << "\n";

    std::string cmd;
    while (true) {
        std::cout << "game> ";
        if (!std::getline(std::cin, cmd)) break;

        rcae::runtime::InputCommand input;
        if (cmd == "quit") {
            input.type = rcae::runtime::InputCommandType::Quit;
        } else if (cmd == "w") {
            input = {rcae::runtime::InputCommandType::Move, 0, -1, 1, ""};
        } else if (cmd == "a") {
            input = {rcae::runtime::InputCommandType::Move, -1, 0, 1, ""};
        } else if (cmd == "s") {
            input = {rcae::runtime::InputCommandType::Move, 0, 1, 1, ""};
        } else if (cmd == "d") {
            input = {rcae::runtime::InputCommandType::Move, 1, 0, 1, ""};
        } else if (cmd == "stop") {
            input.type = rcae::runtime::InputCommandType::Stop;
        } else if (cmd == "tick") {
            input.type = rcae::runtime::InputCommandType::Tick;
        } else if (cmd.rfind("run ", 0) == 0) {
            input.type = rcae::runtime::InputCommandType::RunTicks;
            input.ticks = std::stoi(cmd.substr(4));
        } else if (cmd.rfind("click ", 0) == 0) {
            input.type = rcae::runtime::InputCommandType::ClickButton;
            input.target = cmd.substr(6);
        } else if (cmd == "show") {
            input.type = rcae::runtime::InputCommandType::Show;
        } else {
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

        if (!app.executeCommand(input)) {
            break;
        }

        app.tick();
        std::cout << app.frame() << "\n";
        const auto events = app.consumeEventLog();
        if (!events.empty()) std::cout << events;
    }

    return 0;
}

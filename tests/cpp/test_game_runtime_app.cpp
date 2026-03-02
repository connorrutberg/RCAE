#include "rcae/runtime/GameRuntimeApp.hpp"

#include <cassert>
#include <string>
#include <vector>

int main() {
    rcae::runtime::GameRuntimeApp app;

    const std::vector<std::string> candidates = {
        "games/game_alpha/content/levels/level_runtime_test.map",
        "../games/game_alpha/content/levels/level_runtime_test.map",
    };

    std::string err;
    bool ok = false;
    for (const auto& c : candidates) {
        err.clear();
        if (app.loadLevelFromAsciiFile(c, err)) {
            ok = true;
            break;
        }
    }
    assert(ok);

    const auto initial = app.frame();
    assert(initial.find('@') != std::string::npos);

    // Orchestrator path: move right and tick through trigger tile.
    app.executeCommand({rcae::runtime::InputCommandType::Move, 1, 0, 1, ""});
    app.executeCommand({rcae::runtime::InputCommandType::Move, 1, 0, 1, ""});
    app.executeCommand({rcae::runtime::InputCommandType::Move, 1, 0, 1, ""});
    const auto simEvents = app.consumeEventLog();
    assert(simEvents.find("trigger:") != std::string::npos);

    // UI path: click button added from level tile 'B'.
    app.executeCommand({rcae::runtime::InputCommandType::ClickButton, 0, 0, 1, "button_3_1"});
    const auto uiEvents = app.consumeEventLog();
    assert(uiEvents.find("ui:button_clicked:button_3_1") != std::string::npos);

    return 0;
}

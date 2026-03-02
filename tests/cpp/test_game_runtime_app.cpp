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

    // Move player right into corridor and eventually over trigger tile.
    app.setPlayerVelocity(1, 0);
    for (int i = 0; i < 8; ++i) {
        app.tick();
    }
    const auto events = app.consumeEventLog();
    assert(events.find("trigger:") != std::string::npos);

    return 0;
}

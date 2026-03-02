#include "rcae/runtime/GameRuntimeCAPI.h"

#include <cassert>
#include <string>
#include <vector>

int main() {
    auto* runtime = rcae_game_runtime_create();
    assert(runtime != nullptr);

    const std::vector<const char*> candidates = {
        "games/game_alpha/content/levels/level_runtime_test.map",
        "../games/game_alpha/content/levels/level_runtime_test.map",
    };

    bool loaded = false;
    for (const auto* c : candidates) {
        if (rcae_game_runtime_load_level(runtime, c) == 1) {
            loaded = true;
            break;
        }
    }
    assert(loaded);

    rcae_game_runtime_move(runtime, 1, 0);
    rcae_game_runtime_move(runtime, 1, 0);
    rcae_game_runtime_move(runtime, 1, 0);
    std::string events = rcae_game_runtime_drain_events(runtime);
    assert(events.find("trigger:") != std::string::npos);

    rcae_game_runtime_click_button(runtime, "button_3_1");
    events = rcae_game_runtime_drain_events(runtime);
    assert(events.find("ui:button_clicked:button_3_1") != std::string::npos);

    const std::string frame = rcae_game_runtime_frame(runtime);
    assert(frame.find('@') != std::string::npos);

    rcae_game_runtime_destroy(runtime);
    return 0;
}

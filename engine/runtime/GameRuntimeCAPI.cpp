#include "rcae/runtime/GameRuntimeCAPI.h"

#include "rcae/runtime/GameRuntimeApp.hpp"

#include <string>

struct RCAE_GameRuntime {
    rcae::runtime::GameRuntimeApp app;
};

RCAE_GameRuntime* rcae_game_runtime_create(void) {
    return new RCAE_GameRuntime{};
}

void rcae_game_runtime_destroy(RCAE_GameRuntime* runtime) {
    delete runtime;
}

int rcae_game_runtime_load_level(RCAE_GameRuntime* runtime, const char* path) {
    if (!runtime || !path) return 0;
    std::string err;
    return runtime->app.loadLevelFromAsciiFile(path, err) ? 1 : 0;
}

void rcae_game_runtime_move(RCAE_GameRuntime* runtime, int vx, int vy) {
    if (!runtime) return;
    runtime->app.executeCommand({rcae::runtime::InputCommandType::Move, vx, vy, 1, ""});
}

void rcae_game_runtime_tick(RCAE_GameRuntime* runtime) {
    if (!runtime) return;
    runtime->app.executeCommand({rcae::runtime::InputCommandType::Tick, 0, 0, 1, ""});
}

void rcae_game_runtime_run_ticks(RCAE_GameRuntime* runtime, int ticks) {
    if (!runtime) return;
    runtime->app.executeCommand({rcae::runtime::InputCommandType::RunTicks, 0, 0, ticks, ""});
}

void rcae_game_runtime_click_button(RCAE_GameRuntime* runtime, const char* button_id) {
    if (!runtime || !button_id) return;
    runtime->app.executeCommand({rcae::runtime::InputCommandType::ClickButton, 0, 0, 1, button_id});
}

const char* rcae_game_runtime_frame(RCAE_GameRuntime* runtime) {
    static thread_local std::string frame;
    if (!runtime) {
        frame.clear();
        return frame.c_str();
    }
    frame = runtime->app.frame();
    return frame.c_str();
}

const char* rcae_game_runtime_drain_events(RCAE_GameRuntime* runtime) {
    static thread_local std::string events;
    if (!runtime) {
        events.clear();
        return events.c_str();
    }
    events = runtime->app.consumeEventLog();
    return events.c_str();
}

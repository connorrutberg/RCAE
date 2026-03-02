#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef struct RCAE_GameRuntime RCAE_GameRuntime;

RCAE_GameRuntime* rcae_game_runtime_create(void);
void rcae_game_runtime_destroy(RCAE_GameRuntime* runtime);

int rcae_game_runtime_load_level(RCAE_GameRuntime* runtime, const char* path);
void rcae_game_runtime_move(RCAE_GameRuntime* runtime, int vx, int vy);
void rcae_game_runtime_tick(RCAE_GameRuntime* runtime);
void rcae_game_runtime_run_ticks(RCAE_GameRuntime* runtime, int ticks);
void rcae_game_runtime_click_button(RCAE_GameRuntime* runtime, const char* button_id);

const char* rcae_game_runtime_frame(RCAE_GameRuntime* runtime);
const char* rcae_game_runtime_drain_events(RCAE_GameRuntime* runtime);

#ifdef __cplusplus
}
#endif

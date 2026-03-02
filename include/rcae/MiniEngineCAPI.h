#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef struct RCAE_MiniEngine RCAE_MiniEngine;

typedef struct RCAE_Vec2 {
    int x;
    int y;
} RCAE_Vec2;

RCAE_MiniEngine* rcae_mini_engine_create(void);
void rcae_mini_engine_destroy(RCAE_MiniEngine* engine);

int rcae_mini_engine_create_entity(RCAE_MiniEngine* engine);
int rcae_mini_engine_set_transform(RCAE_MiniEngine* engine, int entity_id, RCAE_Vec2 position);
void rcae_mini_engine_set_velocity(RCAE_MiniEngine* engine, int entity_id, RCAE_Vec2 velocity_per_tick);
void rcae_mini_engine_set_sprite(RCAE_MiniEngine* engine, int entity_id, char glyph);

void rcae_mini_engine_tick(RCAE_MiniEngine* engine);
const char* rcae_mini_engine_render(RCAE_MiniEngine* engine);

#ifdef __cplusplus
}
#endif

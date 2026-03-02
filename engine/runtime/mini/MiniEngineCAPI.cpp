#include "rcae/MiniEngineCAPI.h"

#include "rcae/MiniEngine.hpp"

#include <memory>
#include <string>

struct RCAE_MiniEngine {
    rcae::MiniEngine engine;
};

RCAE_MiniEngine* rcae_mini_engine_create(void) {
    return new RCAE_MiniEngine{};
}

void rcae_mini_engine_destroy(RCAE_MiniEngine* engine) {
    delete engine;
}

int rcae_mini_engine_create_entity(RCAE_MiniEngine* engine) {
    if (!engine) return 0;
    return engine->engine.createEntity();
}

int rcae_mini_engine_set_transform(RCAE_MiniEngine* engine, int entity_id, RCAE_Vec2 position) {
    if (!engine) return 0;
    try {
        engine->engine.setTransform(entity_id, rcae::Transform{rcae::Vec2{position.x, position.y}});
        return 1;
    } catch (...) {
        return 0;
    }
}

void rcae_mini_engine_set_velocity(RCAE_MiniEngine* engine, int entity_id, RCAE_Vec2 velocity_per_tick) {
    if (!engine) return;
    engine->engine.setVelocity(entity_id, rcae::Velocity{rcae::Vec2{velocity_per_tick.x, velocity_per_tick.y}});
}

void rcae_mini_engine_set_sprite(RCAE_MiniEngine* engine, int entity_id, char glyph) {
    if (!engine) return;
    engine->engine.setSprite(entity_id, rcae::Sprite{glyph});
}

void rcae_mini_engine_tick(RCAE_MiniEngine* engine) {
    if (!engine) return;
    engine->engine.tick();
}

const char* rcae_mini_engine_render(RCAE_MiniEngine* engine) {
    static thread_local std::string frame;
    if (!engine) {
        frame.clear();
        return frame.c_str();
    }
    frame = engine->engine.renderToString();
    return frame.c_str();
}

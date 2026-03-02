#include "rcae/MiniEngineCAPI.h"

#include <cassert>
#include <string>

int main() {
    auto* engine = rcae_mini_engine_create();
    assert(engine != nullptr);

    const int player = rcae_mini_engine_create_entity(engine);
    assert(player > 0);

    const int setOk = rcae_mini_engine_set_transform(engine, player, RCAE_Vec2{2, 2});
    assert(setOk == 1);

    rcae_mini_engine_set_velocity(engine, player, RCAE_Vec2{1, 0});
    rcae_mini_engine_set_sprite(engine, player, '@');
    rcae_mini_engine_tick(engine);

    const std::string frame = rcae_mini_engine_render(engine);
    assert(frame.find('@') != std::string::npos);

    rcae_mini_engine_destroy(engine);
    return 0;
}

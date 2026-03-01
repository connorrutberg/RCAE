#include "rcae/scene/SceneWorld.hpp"

#include <cassert>
#include <string>

int main() {
    using namespace rcae;

    scene::SceneWorld world;
    rendering::Camera2D camera{0, 0, 8, 4};

    const auto player = world.createEntity();
    world.setTransform(player, {{1, 1}});
    world.setVelocity(player, {{1, 0}});
    world.setRenderable(player, {2, rendering::Material{"Player", {1.0F, 1.0F, 1.0F}, '@'}});

    const auto ground = world.createEntity();
    world.setTransform(ground, {{2, 1}});
    world.setRenderable(ground, {0, rendering::Material{"Ground", {0.5F, 0.5F, 0.5F}, 'g'}});

    world.tick();

    const auto pos = world.getTransform(player);
    assert(pos.has_value());
    assert((pos->position == scene::Vec2i{2, 1}));

    const std::string frame = world.render(camera, '.');
    assert(frame.find('@') != std::string::npos);
    assert(frame.find('g') == std::string::npos); // layered out at same pixel

    return 0;
}

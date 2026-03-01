#include "rcae/scene/SceneWorld.hpp"

#include <cassert>
#include <string>

int main() {
    using namespace rcae;

    scene::SceneWorld world;
    world.setWorldBounds(8, 4);
    rendering::Camera2D camera{0, 0, 8, 4};

    const auto player = world.createEntity();
    world.setTransform(player, {{1, 1}});
    world.setVelocity(player, {{1, 0}});
    world.setRenderable(player, {2, rendering::Material{"Player", {1.0F, 1.0F, 1.0F}, '@'}});
    world.setCollider(player, {scene::CollisionLayer::Character, scene::CollisionLayer::WorldStatic, true});

    const auto wall = world.createEntity();
    world.setTransform(wall, {{2, 1}});
    world.setRenderable(wall, {0, rendering::Material{"Wall", {0.4F, 0.4F, 0.4F}, '#'}});
    world.setCollider(wall, {scene::CollisionLayer::WorldStatic, scene::CollisionLayer::Character, true});

    // Should be blocked by wall
    world.tick();
    auto pos = world.getTransform(player);
    assert(pos.has_value());
    assert((pos->position == scene::Vec2i{1, 1}));

    // Move wall away; player should advance now
    world.setTransform(wall, {{5, 1}});
    world.tick();
    pos = world.getTransform(player);
    assert(pos.has_value());
    assert((pos->position == scene::Vec2i{2, 1}));

    const std::string frame = world.render(camera, '.');
    assert(frame.find('@') != std::string::npos);

    // Bounds authority: moving beyond world bounds is blocked
    world.setVelocity(player, {{10, 0}});
    world.tick();
    pos = world.getTransform(player);
    assert(pos.has_value());
    assert((pos->position == scene::Vec2i{2, 1}));

    return 0;
}

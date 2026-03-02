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
    world.setCollider(player, {scene::CollisionLayer::Character, scene::CollisionLayer::WorldStatic | scene::CollisionLayer::Trigger, true});

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

    // Bounds authority: moving beyond world bounds is blocked on X
    world.setVelocity(player, {{10, 0}});
    world.tick();
    pos = world.getTransform(player);
    assert(pos.has_value());
    assert((pos->position == scene::Vec2i{2, 1}));

    // Axis-separated movement: X blocked by wall, Y should still advance.
    world.setTransform(player, {{1, 1}});
    world.setVelocity(player, {{1, 1}});
    world.setTransform(wall, {{2, 1}});
    world.tick();
    pos = world.getTransform(player);
    assert(pos.has_value());
    assert((pos->position == scene::Vec2i{1, 2}));

    // Trigger volume should emit event and not block.
    const auto trigger = world.createEntity();
    world.setTransform(trigger, {{3, 2}});
    world.setRenderable(trigger, {1, rendering::Material{"Trigger", {0.2F, 0.9F, 0.2F}, 'T'}});
    world.setCollider(trigger, {scene::CollisionLayer::Trigger, scene::CollisionLayer::Character, false});

    world.setVelocity(player, {{2, 0}}); // from (1,2) to (3,2)
    world.tick();
    pos = world.getTransform(player);
    assert(pos.has_value());
    assert((pos->position == scene::Vec2i{3, 2}));

    const auto events = world.consumeTriggerEvents();
    bool found = false;
    for (const auto& e : events) {
        if (e.moverId == player && e.triggerId == trigger) {
            found = true;
            break;
        }
    }
    assert(found);
    assert(world.consumeTriggerEvents().empty());

    return 0;
}

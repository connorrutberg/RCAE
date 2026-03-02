#include "rcae/scene/SceneWorld.hpp"

#include <iostream>

int main() {
    using namespace rcae;

    scene::SceneWorld world;
    world.setWorldBounds(10, 5);
    rendering::Camera2D camera{0, 0, 10, 5};

    const auto hero = world.createEntity();
    world.setTransform(hero, {{1, 2}});
    world.setVelocity(hero, {{1, 1}});
    world.setRenderable(hero, {2, rendering::Material{"Hero", {1.0F, 1.0F, 1.0F}, '@'}});
    world.setCollider(hero, {scene::CollisionLayer::Character, scene::CollisionLayer::WorldStatic | scene::CollisionLayer::Trigger, true});

    const auto wall = world.createEntity();
    world.setTransform(wall, {{2, 2}});
    world.setRenderable(wall, {1, rendering::Material{"Wall", {0.6F, 0.6F, 0.6F}, '#'}});
    world.setCollider(wall, {scene::CollisionLayer::WorldStatic, scene::CollisionLayer::Character, true});

    const auto trigger = world.createEntity();
    world.setTransform(trigger, {{2, 3}});
    world.setRenderable(trigger, {0, rendering::Material{"Trigger", {0.2F, 0.9F, 0.2F}, 'T'}});
    world.setCollider(trigger, {scene::CollisionLayer::Trigger, scene::CollisionLayer::Character, false});

    std::cout << "Initial:\n" << world.render(camera, '.') << "\n\n";

    for (int i = 0; i < 3; ++i) {
        world.tick();
        std::cout << "Tick " << (i + 1) << " (axis movement + trigger events):\n"
                  << world.render(camera, '.') << "\n";

        const auto events = world.consumeTriggerEvents();
        for (const auto& event : events) {
            std::cout << "  trigger event: mover=" << event.moverId << " trigger=" << event.triggerId << "\n";
        }
        std::cout << "\n";
    }

    return 0;
}

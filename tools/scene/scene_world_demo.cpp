#include "rcae/scene/SceneWorld.hpp"

#include <iostream>

int main() {
    using namespace rcae;

    scene::SceneWorld world;
    rendering::Camera2D camera{0, 0, 10, 5};

    const auto hero = world.createEntity();
    world.setTransform(hero, {{1, 2}});
    world.setVelocity(hero, {{1, 0}});
    world.setRenderable(hero, {2, rendering::Material{"Hero", {1.0F, 1.0F, 1.0F}, '@'}});

    const auto prop = world.createEntity();
    world.setTransform(prop, {{7, 2}});
    world.setRenderable(prop, {1, rendering::Material{"Prop", {0.8F, 0.4F, 0.2F}, 'P'}});

    std::cout << "Initial:\n" << world.render(camera, '.') << "\n\n";

    for (int i = 0; i < 3; ++i) {
        world.tick();
        std::cout << "Tick " << (i + 1) << ":\n" << world.render(camera, '.') << "\n\n";
    }

    return 0;
}

#include "rcae/MiniEngine.hpp"

#include <iostream>

int main() {
    rcae::MiniEngine engine;

    const auto player = engine.createEntity();
    engine.setTransform(player, {{2, 2}});
    engine.setVelocity(player, {{1, 0}});
    engine.setSprite(player, {'@'});

    std::cout << "Initial frame:\n" << engine.renderToString() << "\n\n";

    for (int i = 0; i < 3; ++i) {
        engine.tick();
        std::cout << "Tick " << (i + 1) << ":\n" << engine.renderToString() << "\n\n";
    }

    return 0;
}

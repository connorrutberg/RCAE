#include "rcae/MiniEngine.hpp"

#include <cassert>
#include <iostream>

int main() {
    {
        rcae::MiniEngine engine;
        const auto e = engine.createEntity();
        engine.setTransform(e, {{2, 2}});
        engine.setVelocity(e, {{1, 0}});
        engine.tick();
        const auto pos = engine.getTransform(e);
        assert(pos.has_value());
        assert((pos->position == rcae::Vec2{3, 2}));
    }

    {
        rcae::MiniEngine engine;
        const auto e = engine.createEntity();
        engine.setTransform(e, {{8, 2}});
        engine.setVelocity(e, {{1, 0}}); // into wall at x=9
        engine.tick();
        const auto pos = engine.getTransform(e);
        assert(pos.has_value());
        assert((pos->position == rcae::Vec2{8, 2}));
    }

    {
        rcae::MiniEngine engine;
        const auto e = engine.createEntity();
        engine.setTransform(e, {{3, 1}});
        engine.setSprite(e, {'P'});
        const auto frame = engine.renderToString();
        assert(frame.find('P') != std::string::npos);
    }

    std::cout << "All MiniEngine tests passed\n";
    return 0;
}

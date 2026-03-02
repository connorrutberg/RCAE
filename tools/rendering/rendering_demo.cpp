#include "rcae/rendering/Renderer.hpp"

#include <iostream>

int main() {
    using namespace rcae::rendering;

    Renderer2D renderer;
    Camera2D camera{0, 0, 12, 6};

    std::vector<SpriteInstance> scene {
        {2, 2, 0, Material{"Floor", {0.4F, 0.4F, 0.4F}, '_'}},
        {3, 2, 0, Material{"Floor", {0.4F, 0.4F, 0.4F}, '_'}},
        {4, 2, 0, Material{"Floor", {0.4F, 0.4F, 0.4F}, '_'}},
        {3, 2, 2, Material{"Hero", {1.0F, 1.0F, 1.0F}, '@'}},
        {8, 3, 1, Material{"Prop", {0.8F, 0.5F, 0.2F}, 'P'}},
    };

    std::cout << renderer.renderFrame(camera, scene, '.') << '\n';
    return 0;
}

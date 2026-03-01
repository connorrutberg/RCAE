#include "rcae/rendering/Renderer.hpp"

#include <cassert>
#include <string>
#include <vector>

int main() {
    using namespace rcae::rendering;

    Renderer2D renderer;
    Camera2D camera{0, 0, 6, 4};

    const std::vector<SpriteInstance> sprites {
        {1, 1, 0, Material{"Ground", {0.7F, 0.7F, 0.7F}, 'g'}},
        {1, 1, 2, Material{"Player", {1.0F, 1.0F, 1.0F}, '@'}},
        {4, 2, 1, Material{"Crate", {0.8F, 0.4F, 0.1F}, 'c'}},
        {8, 8, 1, Material{"Offscreen", {1.0F, 0.0F, 0.0F}, 'x'}},
    };

    const std::string frame = renderer.renderFrame(camera, sprites, '.');

    assert(frame.find('@') != std::string::npos);
    assert(frame.find('c') != std::string::npos);
    assert(frame.find('x') == std::string::npos);

    // top-left should remain cleared
    assert(frame.rfind("......\n", 0) == 0);

    return 0;
}

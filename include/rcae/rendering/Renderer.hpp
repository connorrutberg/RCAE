#pragma once

#include <string>
#include <vector>

namespace rcae::rendering {

struct Color {
    float r {1.0F};
    float g {1.0F};
    float b {1.0F};
};

struct Material {
    std::string name;
    Color albedo;
    char glyph {'#'};
};

struct SpriteInstance {
    int x {0};
    int y {0};
    int layer {0};
    Material material;
};

struct Camera2D {
    int x {0};
    int y {0};
    int width {16};
    int height {9};
};

class Renderer2D {
public:
    std::string renderFrame(const Camera2D& camera,
                            const std::vector<SpriteInstance>& instances,
                            char clearGlyph = '.');

private:
    static bool inView(const Camera2D& camera, const SpriteInstance& instance);
};

} // namespace rcae::rendering

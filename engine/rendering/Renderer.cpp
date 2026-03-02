#include "rcae/rendering/Renderer.hpp"

#include <algorithm>
#include <sstream>

namespace rcae::rendering {

std::string Renderer2D::renderFrame(const Camera2D& camera,
                                    const std::vector<SpriteInstance>& instances,
                                    char clearGlyph) const {
                                    char clearGlyph) {
    if (camera.width <= 0 || camera.height <= 0) {
        return "";
    }

    std::vector<std::string> frame(static_cast<size_t>(camera.height), std::string(static_cast<size_t>(camera.width), clearGlyph));
    std::vector<int> depth(static_cast<size_t>(camera.width * camera.height), -2147483647);

    std::vector<SpriteInstance> sorted = instances;
    std::stable_sort(sorted.begin(), sorted.end(), [](const SpriteInstance& a, const SpriteInstance& b) {
        return a.layer < b.layer;
    });

    for (const auto& instance : sorted) {
        if (!inView(camera, instance)) {
            continue;
        }

        const int sx = instance.x - camera.x;
        const int sy = instance.y - camera.y;

        const auto index = static_cast<size_t>(sy * camera.width + sx);
        if (instance.layer >= depth[index]) {
            frame[static_cast<size_t>(sy)][static_cast<size_t>(sx)] = instance.material.glyph;
            depth[index] = instance.layer;
        }
    }

    std::ostringstream out;
    for (size_t y = 0; y < frame.size(); ++y) {
        out << frame[y];
        if (y + 1 < frame.size()) {
            out << '\n';
        }
    }
    return out.str();
}

bool Renderer2D::inView(const Camera2D& camera, const SpriteInstance& instance) {
    return instance.x >= camera.x && instance.y >= camera.y
        && instance.x < camera.x + camera.width
        && instance.y < camera.y + camera.height;
}

} // namespace rcae::rendering

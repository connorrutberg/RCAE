#pragma once

#include "rcae/rendering/Renderer.hpp"

#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

namespace rcae::scene {

using EntityId = int;

struct Vec2i {
    int x {0};
    int y {0};

    bool operator==(const Vec2i& other) const = default;
};

struct Transform2D {
    Vec2i position;
};

struct Velocity2D {
    Vec2i deltaPerTick;
};

struct Renderable2D {
    int layer {0};
    rendering::Material material;
};

class SceneWorld {
public:
    explicit SceneWorld(rendering::Renderer2D renderer = {});

    EntityId createEntity();

    void setTransform(EntityId id, Transform2D transform);
    void setVelocity(EntityId id, Velocity2D velocity);
    void setRenderable(EntityId id, Renderable2D renderable);

    std::optional<Transform2D> getTransform(EntityId id) const;

    void tick();
    std::string render(const rendering::Camera2D& camera, char clearGlyph = '.') const;

private:
    std::vector<rendering::SpriteInstance> buildRenderInstances() const;

    int nextEntityId_ {1};
    rendering::Renderer2D renderer_;
    std::unordered_map<EntityId, Transform2D> transforms_;
    std::unordered_map<EntityId, Velocity2D> velocities_;
    std::unordered_map<EntityId, Renderable2D> renderables_;
};

} // namespace rcae::scene

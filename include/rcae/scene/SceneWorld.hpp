#pragma once

#include "rcae/rendering/Renderer.hpp"

#include <cstdint>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

namespace rcae::scene {

using EntityId = int;
using CollisionMask = std::uint32_t;

namespace CollisionLayer {
constexpr CollisionMask None = 0;
constexpr CollisionMask WorldStatic = 1u << 0;
constexpr CollisionMask Character = 1u << 1;
constexpr CollisionMask Trigger = 1u << 2;
} // namespace CollisionLayer

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

struct Collider2D {
    CollisionMask layer {CollisionLayer::None};
    CollisionMask collidesWithMask {CollisionLayer::None};
    bool blocking {true};
};

struct TriggerEvent {
    EntityId moverId {0};
    EntityId triggerId {0};
};

class SceneWorld {
public:
    explicit SceneWorld(rendering::Renderer2D renderer = {});

    EntityId createEntity();

    void setWorldBounds(int width, int height);
    void setTransform(EntityId id, Transform2D transform);
    void setVelocity(EntityId id, Velocity2D velocity);
    void setRenderable(EntityId id, Renderable2D renderable);
    void setCollider(EntityId id, Collider2D collider);

    std::optional<Transform2D> getTransform(EntityId id) const;
    std::vector<TriggerEvent> consumeTriggerEvents();

    void tick();
    std::string render(const rendering::Camera2D& camera, char clearGlyph = '.') const;

private:
    bool inWorldBounds(const Vec2i& position) const;
    bool isBlockingCollision(EntityId movingId, const Vec2i& destination) const;
    void collectTriggerEvents(EntityId movingId, const Vec2i& destination);
    std::vector<rendering::SpriteInstance> buildRenderInstances() const;

    int nextEntityId_ {1};
    int worldWidth_ {64};
    int worldHeight_ {64};

    rendering::Renderer2D renderer_;
    std::unordered_map<EntityId, Transform2D> transforms_;
    std::unordered_map<EntityId, Velocity2D> velocities_;
    std::unordered_map<EntityId, Renderable2D> renderables_;
    std::unordered_map<EntityId, Collider2D> colliders_;
    std::vector<TriggerEvent> pendingTriggerEvents_;
};

} // namespace rcae::scene

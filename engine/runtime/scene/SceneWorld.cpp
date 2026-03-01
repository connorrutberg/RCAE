#include "rcae/scene/SceneWorld.hpp"

namespace rcae::scene {

SceneWorld::SceneWorld(rendering::Renderer2D renderer) : renderer_(std::move(renderer)) {}

EntityId SceneWorld::createEntity() {
    return nextEntityId_++;
}

void SceneWorld::setWorldBounds(int width, int height) {
    if (width > 0) worldWidth_ = width;
    if (height > 0) worldHeight_ = height;
}

void SceneWorld::setTransform(EntityId id, Transform2D transform) {
    transforms_[id] = transform;
}

void SceneWorld::setVelocity(EntityId id, Velocity2D velocity) {
    velocities_[id] = velocity;
}

void SceneWorld::setRenderable(EntityId id, Renderable2D renderable) {
    renderables_[id] = std::move(renderable);
}

void SceneWorld::setCollider(EntityId id, Collider2D collider) {
    colliders_[id] = collider;
}

std::optional<Transform2D> SceneWorld::getTransform(EntityId id) const {
    if (auto it = transforms_.find(id); it != transforms_.end()) {
        return it->second;
    }
    return std::nullopt;
}

void SceneWorld::tick() {
    for (const auto& [id, velocity] : velocities_) {
        auto transformIt = transforms_.find(id);
        if (transformIt == transforms_.end()) {
            continue;
        }

        const Vec2i destination {
            transformIt->second.position.x + velocity.deltaPerTick.x,
            transformIt->second.position.y + velocity.deltaPerTick.y,
        };

        if (canMoveTo(id, destination)) {
            transformIt->second.position = destination;
        }
    }
}

std::string SceneWorld::render(const rendering::Camera2D& camera, char clearGlyph) const {
    return renderer_.renderFrame(camera, buildRenderInstances(), clearGlyph);
}

bool SceneWorld::inWorldBounds(const Vec2i& position) const {
    return position.x >= 0 && position.y >= 0
        && position.x < worldWidth_ && position.y < worldHeight_;
}

bool SceneWorld::canMoveTo(EntityId movingId, const Vec2i& destination) const {
    if (!inWorldBounds(destination)) {
        return false;
    }

    const auto movingColliderIt = colliders_.find(movingId);
    if (movingColliderIt == colliders_.end() || !movingColliderIt->second.blocking) {
        return true;
    }

    const auto& movingCollider = movingColliderIt->second;

    for (const auto& [otherId, otherTransform] : transforms_) {
        if (otherId == movingId || otherTransform.position != destination) {
            continue;
        }

        const auto otherColliderIt = colliders_.find(otherId);
        if (otherColliderIt == colliders_.end()) {
            continue;
        }

        const auto& otherCollider = otherColliderIt->second;
        if (!otherCollider.blocking) {
            continue;
        }

        const bool movingHitsOther = (movingCollider.collidesWithMask & otherCollider.layer) != 0;
        const bool otherHitsMoving = (otherCollider.collidesWithMask & movingCollider.layer) != 0;
        if (movingHitsOther || otherHitsMoving) {
            return false;
        }
    }

    return true;
}

std::vector<rendering::SpriteInstance> SceneWorld::buildRenderInstances() const {
    std::vector<rendering::SpriteInstance> instances;
    instances.reserve(renderables_.size());

    for (const auto& [id, renderable] : renderables_) {
        const auto transformIt = transforms_.find(id);
        if (transformIt == transforms_.end()) {
            continue;
        }

        instances.push_back(rendering::SpriteInstance{
            transformIt->second.position.x,
            transformIt->second.position.y,
            renderable.layer,
            renderable.material,
        });
    }

    return instances;
}

} // namespace rcae::scene

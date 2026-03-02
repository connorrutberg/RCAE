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

std::vector<TriggerEvent> SceneWorld::consumeTriggerEvents() {
    auto out = pendingTriggerEvents_;
    pendingTriggerEvents_.clear();
    return out;
}

void SceneWorld::tick() {
    for (const auto& [id, velocity] : velocities_) {
        auto transformIt = transforms_.find(id);
        if (transformIt == transforms_.end()) {
            continue;
        }

        // Axis-separated movement for predictable sliding against blockers.
        Vec2i step = transformIt->second.position;
        if (velocity.deltaPerTick.x != 0) {
            const Vec2i xTry {step.x + velocity.deltaPerTick.x, step.y};
            if (inWorldBounds(xTry) && !isBlockingCollision(id, xTry)) {
                step.x = xTry.x;
            }
            collectTriggerEvents(id, xTry);
        }

        if (velocity.deltaPerTick.y != 0) {
            const Vec2i yTry {step.x, step.y + velocity.deltaPerTick.y};
            if (inWorldBounds(yTry) && !isBlockingCollision(id, yTry)) {
                step.y = yTry.y;
            }
            collectTriggerEvents(id, yTry);
        }

        transformIt->second.position = step;
    }
}

std::string SceneWorld::render(const rendering::Camera2D& camera, char clearGlyph) const {
    return renderer_.renderFrame(camera, buildRenderInstances(), clearGlyph);
}

bool SceneWorld::inWorldBounds(const Vec2i& position) const {
    return position.x >= 0 && position.y >= 0
        && position.x < worldWidth_ && position.y < worldHeight_;
}

bool SceneWorld::isBlockingCollision(EntityId movingId, const Vec2i& destination) const {
    const auto movingColliderIt = colliders_.find(movingId);
    if (movingColliderIt == colliders_.end() || !movingColliderIt->second.blocking) {
        return false;
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
            return true;
        }
    }

    return false;
}

void SceneWorld::collectTriggerEvents(EntityId movingId, const Vec2i& destination) {
    if (!inWorldBounds(destination)) {
        return;
    }

    const auto movingColliderIt = colliders_.find(movingId);
    if (movingColliderIt == colliders_.end()) {
        return;
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
        const bool layerMatch = (movingCollider.collidesWithMask & otherCollider.layer) != 0
            || (otherCollider.collidesWithMask & movingCollider.layer) != 0;
        if (!layerMatch) {
            continue;
        }

        const bool involvesTrigger = (movingCollider.layer & CollisionLayer::Trigger) != 0
            || (otherCollider.layer & CollisionLayer::Trigger) != 0;
        if (involvesTrigger) {
            pendingTriggerEvents_.push_back(TriggerEvent{movingId, otherId});
        }
    }
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

#include "rcae/scene/SceneWorld.hpp"

namespace rcae::scene {

SceneWorld::SceneWorld(rendering::Renderer2D renderer) : renderer_(std::move(renderer)) {}

EntityId SceneWorld::createEntity() {
    return nextEntityId_++;
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
        transformIt->second.position.x += velocity.deltaPerTick.x;
        transformIt->second.position.y += velocity.deltaPerTick.y;
    }
}

std::string SceneWorld::render(const rendering::Camera2D& camera, char clearGlyph) const {
    return renderer_.renderFrame(camera, buildRenderInstances(), clearGlyph);
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

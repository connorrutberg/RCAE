#include "rcae/MiniEngine.hpp"

#include <sstream>
#include <stdexcept>

namespace rcae {

MiniEngine::MiniEngine() {
    loadTileMap({
        "##########",
        "#........#",
        "#........#",
        "#........#",
        "##########",
    });
}

EntityId MiniEngine::createEntity() {
    return nextEntityId_++;
}

void MiniEngine::setTransform(EntityId id, Transform transform) {
    if (!canOccupy(transform.position)) {
        throw std::runtime_error("Entity transform is outside walkable map bounds");
    }
    transforms_[id] = transform;
}

void MiniEngine::setVelocity(EntityId id, Velocity velocity) {
    velocities_[id] = velocity;
}

void MiniEngine::setSprite(EntityId id, Sprite sprite) {
    sprites_[id] = sprite;
}

std::optional<Transform> MiniEngine::getTransform(EntityId id) const {
    if (auto it = transforms_.find(id); it != transforms_.end()) {
        return it->second;
    }
    return std::nullopt;
}

void MiniEngine::loadTileMap(std::vector<std::string> mapRows) {
    if (mapRows.empty()) {
        throw std::runtime_error("Tile map cannot be empty");
    }
    const auto width = mapRows.front().size();
    if (width == 0) {
        throw std::runtime_error("Tile map rows cannot be empty");
    }
    for (const auto& row : mapRows) {
        if (row.size() != width) {
            throw std::runtime_error("Tile map must be rectangular");
        }
    }
    tileMap_ = std::move(mapRows);
}

void MiniEngine::tick() {
    for (const auto& [id, velocity] : velocities_) {
        auto transformIt = transforms_.find(id);
        if (transformIt == transforms_.end()) continue;

        const Vec2 next {
            transformIt->second.position.x + velocity.deltaPerTick.x,
            transformIt->second.position.y + velocity.deltaPerTick.y,
        };

        if (canOccupy(next)) {
            transformIt->second.position = next;
        }
    }
}

std::string MiniEngine::renderToString() const {
    auto frame = tileMap_;
    for (const auto& [id, transform] : transforms_) {
        if (!canOccupy(transform.position)) continue;
        const auto spriteIt = sprites_.find(id);
        const char glyph = (spriteIt != sprites_.end()) ? spriteIt->second.glyph : 'E';
        frame[static_cast<size_t>(transform.position.y)][static_cast<size_t>(transform.position.x)] = glyph;
    }

    std::ostringstream out;
    for (size_t i = 0; i < frame.size(); ++i) {
        out << frame[i];
        if (i + 1 < frame.size()) out << '\n';
    }
    return out.str();
}

bool MiniEngine::canOccupy(const Vec2& position) const {
    if (tileMap_.empty()) return false;
    if (position.y < 0 || position.x < 0) return false;

    const auto maxY = static_cast<int>(tileMap_.size());
    const auto maxX = static_cast<int>(tileMap_.front().size());
    if (position.y >= maxY || position.x >= maxX) return false;

    return tileMap_[static_cast<size_t>(position.y)][static_cast<size_t>(position.x)] != '#';
}

} // namespace rcae

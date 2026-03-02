#pragma once

#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

namespace rcae {

using EntityId = int;

struct Vec2 {
    int x {0};
    int y {0};

    bool operator==(const Vec2& other) const = default;
};

struct Transform {
    Vec2 position;
};

struct Velocity {
    Vec2 deltaPerTick;
};

struct Sprite {
    char glyph {'?'};
};

class MiniEngine {
public:
    MiniEngine();

    EntityId createEntity();
    void setTransform(EntityId id, Transform transform);
    void setVelocity(EntityId id, Velocity velocity);
    void setSprite(EntityId id, Sprite sprite);

    std::optional<Transform> getTransform(EntityId id) const;

    void loadTileMap(std::vector<std::string> mapRows);
    void tick();
    std::string renderToString() const;

private:
    bool canOccupy(const Vec2& position) const;

    int nextEntityId_ {1};
    std::unordered_map<EntityId, Transform> transforms_;
    std::unordered_map<EntityId, Velocity> velocities_;
    std::unordered_map<EntityId, Sprite> sprites_;
    std::vector<std::string> tileMap_;
};

} // namespace rcae

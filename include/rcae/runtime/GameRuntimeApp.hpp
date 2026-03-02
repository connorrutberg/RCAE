#pragma once

#include "rcae/scene/SceneWorld.hpp"

#include <string>

namespace rcae::runtime {

class GameRuntimeApp {
public:
    GameRuntimeApp();

    bool loadLevelFromAsciiFile(const std::string& path, std::string& outError);
    void setPlayerVelocity(int vx, int vy);
    void tick();
    std::string frame() const;
    std::string consumeEventLog();

private:
    scene::EntityId playerId_ {0};
    rendering::Camera2D camera_;
    scene::SceneWorld world_;
    std::string eventLog_;
};

} // namespace rcae::runtime

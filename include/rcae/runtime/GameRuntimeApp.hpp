#pragma once

#include "rcae/runtime/FrameOrchestrator.hpp"
#include "rcae/runtime/interfaces/RuntimeInterfaces.hpp"
#include "rcae/scene/SceneWorld.hpp"

#include <memory>
#include "rcae/scene/SceneWorld.hpp"

#include <string>

namespace rcae::runtime {

class GameRuntimeApp {
public:
    GameRuntimeApp();
    ~GameRuntimeApp();

    bool loadLevelFromAsciiFile(const std::string& path, std::string& outError);
    bool executeCommand(const InputCommand& command);


    bool loadLevelFromAsciiFile(const std::string& path, std::string& outError);
    void setPlayerVelocity(int vx, int vy);
    void tick();
    std::string frame() const;
    std::string consumeEventLog();

private:
    class SceneWorldSimulation;
    class RuntimeEventBus;
    class RuntimeUISystem;

    std::unique_ptr<SceneWorldSimulation> simulation_;
    std::unique_ptr<RuntimeEventBus> eventBus_;
    std::unique_ptr<RuntimeUISystem> ui_;
    std::unique_ptr<FrameOrchestrator> orchestrator_;
    scene::EntityId playerId_ {0};
    rendering::Camera2D camera_;
    scene::SceneWorld world_;
    std::string eventLog_;
};

} // namespace rcae::runtime

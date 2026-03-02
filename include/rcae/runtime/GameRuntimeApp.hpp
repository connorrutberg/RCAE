#pragma once

#include "rcae/runtime/FrameOrchestrator.hpp"
#include "rcae/runtime/interfaces/RuntimeInterfaces.hpp"
#include "rcae/scene/SceneWorld.hpp"

#include <memory>
#include <string>

namespace rcae::runtime {

class GameRuntimeApp {
public:
    GameRuntimeApp();
    ~GameRuntimeApp();

    bool loadLevelFromAsciiFile(const std::string& path, std::string& outError);
    bool executeCommand(const InputCommand& command);

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
};

} // namespace rcae::runtime

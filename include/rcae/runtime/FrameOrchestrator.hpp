#pragma once

#include "rcae/runtime/interfaces/RuntimeInterfaces.hpp"

namespace rcae::runtime {

class FrameOrchestrator {
public:
    FrameOrchestrator(IWorldSimulation& world, IUISystem& ui, IEventBus& eventBus);

    bool execute(const InputCommand& command);

private:
    IWorldSimulation& world_;
    IUISystem& ui_;
    IEventBus& eventBus_;
};

} // namespace rcae::runtime

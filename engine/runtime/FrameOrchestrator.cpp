#include "rcae/runtime/FrameOrchestrator.hpp"

namespace rcae::runtime {

FrameOrchestrator::FrameOrchestrator(IWorldSimulation& world, IUISystem& ui, IEventBus& eventBus)
    : world_(world), ui_(ui), eventBus_(eventBus) {}

bool FrameOrchestrator::execute(const InputCommand& command) {
    switch (command.type) {
    case InputCommandType::Move:
        world_.setPlayerVelocity(command.vx, command.vy);
        world_.tick();
        break;
    case InputCommandType::Stop:
        world_.setPlayerVelocity(0, 0);
        world_.tick();
        break;
    case InputCommandType::Tick:
        world_.tick();
        break;
    case InputCommandType::RunTicks:
        for (int i = 0; i < command.ticks; ++i) {
            world_.tick();
        }
        break;
    case InputCommandType::ClickButton:
        if (!ui_.clickButton(command.target, eventBus_)) {
            eventBus_.publish("ui:button_missing:" + command.target);
        }
        break;
    case InputCommandType::Show:
        break;
    case InputCommandType::Quit:
        return false;
    }

    for (auto& e : world_.consumeSimulationEvents()) {
        eventBus_.publish(std::move(e));
    }
    return true;
}

} // namespace rcae::runtime

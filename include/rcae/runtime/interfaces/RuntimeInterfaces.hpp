#pragma once

#include <optional>
#include <string>
#include <vector>

namespace rcae::runtime {

enum class InputCommandType {
    Move,
    Stop,
    Tick,
    RunTicks,
    ClickButton,
    Show,
    Quit,
};

struct InputCommand {
    InputCommandType type {InputCommandType::Tick};
    int vx {0};
    int vy {0};
    int ticks {1};
    std::string target;
};

class IWorldSimulation {
public:
    virtual ~IWorldSimulation() = default;
    virtual void setPlayerVelocity(int vx, int vy) = 0;
    virtual void tick() = 0;
    virtual std::string frame() const = 0;
    virtual std::vector<std::string> consumeSimulationEvents() = 0;
};

class IEventBus {
public:
    virtual ~IEventBus() = default;
    virtual void publish(std::string event) = 0;
    virtual std::vector<std::string> drain() = 0;
};

class IUISystem {
public:
    virtual ~IUISystem() = default;
    virtual void addButton(std::string id, std::string label) = 0;
    virtual bool clickButton(const std::string& id, IEventBus& eventBus) = 0;
};

class IInputSource {
public:
    virtual ~IInputSource() = default;
    virtual std::optional<InputCommand> poll() = 0;
};

} // namespace rcae::runtime

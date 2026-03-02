#include "rcae/runtime/GameRuntimeApp.hpp"

#include <fstream>
#include <memory>
#include <sstream>
#include <unordered_map>
#include <vector>

namespace rcae::runtime {

class GameRuntimeApp::SceneWorldSimulation : public IWorldSimulation {
public:
    SceneWorldSimulation() : camera_{0, 0, 16, 10} {}

    bool loadLevelFromAsciiFile(const std::string& path, std::string& outError, IUISystem& ui) {
        std::ifstream in(path);
        if (!in) {
            outError = "Could not open level file: " + path;
            return false;
        }

        std::vector<std::string> rows;
        std::string line;
        while (std::getline(in, line)) {
            if (!line.empty()) rows.push_back(line);
        }
        if (rows.empty()) {
            outError = "Level file is empty";
            return false;
        }

        const int height = static_cast<int>(rows.size());
        const int width = static_cast<int>(rows.front().size());
        for (const auto& row : rows) {
            if (static_cast<int>(row.size()) != width) {
                outError = "Level rows must be rectangular";
                return false;
            }
        }

        world_ = scene::SceneWorld{};
        world_.setWorldBounds(width, height);
        camera_ = rendering::Camera2D{0, 0, width, height};
        playerId_ = 0;

        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                const char c = rows[static_cast<size_t>(y)][static_cast<size_t>(x)];
                if (c == '#') {
                    const auto id = world_.createEntity();
                    world_.setTransform(id, {{x, y}});
                    world_.setRenderable(id, {1, rendering::Material{"Wall", {0.5F, 0.5F, 0.5F}, '#'}});
                    world_.setCollider(id, {scene::CollisionLayer::WorldStatic, scene::CollisionLayer::Character, true});
                } else if (c == 'T') {
                    const auto id = world_.createEntity();
                    world_.setTransform(id, {{x, y}});
                    world_.setRenderable(id, {0, rendering::Material{"Trigger", {0.2F, 0.9F, 0.2F}, 'T'}});
                    world_.setCollider(id, {scene::CollisionLayer::Trigger, scene::CollisionLayer::Character, false});
                } else if (c == 'P') {
                    playerId_ = world_.createEntity();
                    world_.setTransform(playerId_, {{x, y}});
                    world_.setRenderable(playerId_, {3, rendering::Material{"Player", {1.0F, 1.0F, 1.0F}, '@'}});
                    world_.setCollider(playerId_, {scene::CollisionLayer::Character, scene::CollisionLayer::WorldStatic | scene::CollisionLayer::Trigger, true});
                    world_.setVelocity(playerId_, {{0, 0}});
                } else if (c == 'B') {
                    const auto id = world_.createEntity();
                    world_.setTransform(id, {{x, y}});
                    world_.setRenderable(id, {2, rendering::Material{"Button", {0.2F, 0.5F, 1.0F}, 'B'}});
                    const std::string buttonId = "button_" + std::to_string(x) + "_" + std::to_string(y);
                    ui.addButton(buttonId, "Action Button");
                }
            }
        }

        if (playerId_ == 0) {
            outError = "Level requires a player start tile 'P'";
            return false;
        }
        return true;
    }

    void setPlayerVelocity(int vx, int vy) override {
        if (playerId_ == 0) return;
        world_.setVelocity(playerId_, {{vx, vy}});
    }

    void tick() override {
        world_.tick();
    }

    std::string frame() const override {
        return world_.render(camera_, '.');
    }

    std::vector<std::string> consumeSimulationEvents() override {
        std::vector<std::string> out;
        for (const auto& e : world_.consumeTriggerEvents()) {
            out.push_back("trigger:mover=" + std::to_string(e.moverId) + ",target=" + std::to_string(e.triggerId));
        }
        return out;
    }

private:
    scene::EntityId playerId_ {0};
    rendering::Camera2D camera_;
    scene::SceneWorld world_;
};

class GameRuntimeApp::RuntimeEventBus : public IEventBus {
public:
    void publish(std::string event) override {
        events_.push_back(std::move(event));
    }

    std::vector<std::string> drain() override {
        auto out = events_;
        events_.clear();
        return out;
    }

private:
    std::vector<std::string> events_;
};

class GameRuntimeApp::RuntimeUISystem : public IUISystem {
public:
    void addButton(std::string id, std::string label) override {
        buttons_[std::move(id)] = std::move(label);
    }

    bool clickButton(const std::string& id, IEventBus& eventBus) override {
        const auto it = buttons_.find(id);
        if (it == buttons_.end()) {
            return false;
        }
        eventBus.publish("ui:button_clicked:" + id + ":" + it->second);
        return true;
    }

private:
    std::unordered_map<std::string, std::string> buttons_;
};

GameRuntimeApp::GameRuntimeApp()
    : simulation_(std::make_unique<SceneWorldSimulation>()),
      eventBus_(std::make_unique<RuntimeEventBus>()),
      ui_(std::make_unique<RuntimeUISystem>()),
      orchestrator_(std::make_unique<FrameOrchestrator>(*simulation_, *ui_, *eventBus_)) {}

GameRuntimeApp::~GameRuntimeApp() = default;

bool GameRuntimeApp::loadLevelFromAsciiFile(const std::string& path, std::string& outError) {
    return simulation_->loadLevelFromAsciiFile(path, outError, *ui_);
}

bool GameRuntimeApp::executeCommand(const InputCommand& command) {
    return orchestrator_->execute(command);
}

void GameRuntimeApp::setPlayerVelocity(int vx, int vy) {
    executeCommand(InputCommand{InputCommandType::Move, vx, vy, 1, ""});
}

void GameRuntimeApp::tick() {
    executeCommand(InputCommand{InputCommandType::Tick, 0, 0, 1, ""});
}

std::string GameRuntimeApp::frame() const {
    return simulation_->frame();
}

std::string GameRuntimeApp::consumeEventLog() {
    std::ostringstream out;
    for (const auto& e : eventBus_->drain()) {
        out << e << "\n";
    }
    return out.str();
}

} // namespace rcae::runtime

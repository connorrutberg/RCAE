#include "rcae/runtime/GameRuntimeApp.hpp"

#include <fstream>
#include <sstream>
#include <vector>

namespace rcae::runtime {

GameRuntimeApp::GameRuntimeApp() : camera_{0, 0, 16, 10} {}

bool GameRuntimeApp::loadLevelFromAsciiFile(const std::string& path, std::string& outError) {
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
    eventLog_.clear();

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
            }
        }
    }

    if (playerId_ == 0) {
        outError = "Level requires a player start tile 'P'";
        return false;
    }

    return true;
}

void GameRuntimeApp::setPlayerVelocity(int vx, int vy) {
    if (playerId_ == 0) return;
    world_.setVelocity(playerId_, {{vx, vy}});
}

void GameRuntimeApp::tick() {
    world_.tick();
    const auto events = world_.consumeTriggerEvents();
    if (!events.empty()) {
        std::ostringstream out;
        for (const auto& e : events) {
            out << "trigger:mover=" << e.moverId << ",target=" << e.triggerId << "\n";
        }
        eventLog_ += out.str();
    }
}

std::string GameRuntimeApp::frame() const {
    return world_.render(camera_, '.');
}

std::string GameRuntimeApp::consumeEventLog() {
    auto out = eventLog_;
    eventLog_.clear();
    return out;
}

} // namespace rcae::runtime

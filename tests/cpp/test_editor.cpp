#include "rcae/EditorApp.hpp"

#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>

int main() {
    rcae::EditorApp app;
    app.loadDefaultScripts();

    {
        const auto entities = app.handle("entities");
        assert(entities.find("switch_01") != std::string::npos);
    }

    {
        app.handle("select switch_01");
        const auto scripts = app.handle("scripts Door");
        assert(scripts.find("DoorTrigger(C#)") != std::string::npos);

        app.handle("addscript DoorTrigger");
        app.handle("set DoorTrigger OpenDelay 1.5");
        const auto inspect = app.handle("inspect");
        assert(inspect.find("OpenDelay=1.5") != std::string::npos);
    }

    {
        const auto imported = app.handle("import props/barrel.fbx");
        assert(imported.find("imported") != std::string::npos);

        const std::filesystem::path out = "packages/cooked/test_bundle.json";
        const auto exported = app.handle("export " + out.string());
        assert(exported.find("exported") != std::string::npos);
        assert(std::filesystem::exists(out));
    }

    {
        rcae::MountResolver resolver({"engine/common", "shared/franchise_core", "games/game_alpha/content", "packages/overrides_dev"});
        std::unordered_map<std::string, std::unordered_set<std::string>> mounted {
            {"engine/common", {"textures/wall_a.png"}},
            {"games/game_alpha/content", {"textures/wall_a.png"}},
            {"packages/overrides_dev", {"textures/wall_a.png"}},
        };
        const auto resolved = resolver.resolve("textures/wall_a.png", mounted);
        assert(resolved == "packages/overrides_dev/textures/wall_a.png");
    }

    std::cout << "All C++ tests passed\n";
    return 0;
}

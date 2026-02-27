#include "rcae/EditorApp.hpp"

#include <algorithm>
#include <cctype>
#include <filesystem>
#include <fstream>
#include <regex>
#include <sstream>
#include <stdexcept>

namespace rcae {

namespace {
std::string toLower(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    return s;
}

std::string trim(const std::string& s) {
    const auto begin = s.find_first_not_of(" \t\r\n");
    if (begin == std::string::npos) return "";
    const auto end = s.find_last_not_of(" \t\r\n");
    return s.substr(begin, end - begin + 1);
}
} // namespace

void ScriptCatalog::registerCppComponent(const ScriptComponentDef& def) {
    defs_.push_back(def);
}

void ScriptCatalog::loadCSharpComponentFile(const std::string& path) {
    std::ifstream in(path);
    if (!in) {
        throw std::runtime_error("Could not open C# script file: " + path);
    }

    ScriptComponentDef def;
    def.language = "C#";
    def.category = "script";

    std::string line;
    std::string pendingName;
    std::string pendingType;

    const std::regex classRegex(R"(class\s+([A-Za-z_][A-Za-z0-9_]*))");
    const std::regex exposeRegex(R"REGEX(\[ExposeProperty\("([^"]+)"\s*,\s*"([^"]+)"\)\])REGEX");
    const std::regex propRegex(R"(public\s+[A-Za-z0-9_<>]+\s+([A-Za-z_][A-Za-z0-9_]*)\s*\{)");

    while (std::getline(in, line)) {
        std::smatch m;
        if (std::regex_search(line, m, classRegex)) {
            def.name = m[1];
        }
        if (std::regex_search(line, m, exposeRegex)) {
            pendingName = m[1];
            pendingType = m[2];
        } else if (!pendingName.empty() && std::regex_search(line, m, propRegex)) {
            def.properties.push_back(ScriptProperty{pendingName, pendingType, ""});
            pendingName.clear();
            pendingType.clear();
        }
    }

    if (def.name.empty()) {
        throw std::runtime_error("No class found in C# script file: " + path);
    }

    defs_.push_back(def);
}

std::vector<ScriptComponentDef> ScriptCatalog::search(const std::string& query) const {
    const auto q = toLower(query);
    std::vector<ScriptComponentDef> out;
    for (const auto& d : defs_) {
        if (q.empty() || toLower(d.name).find(q) != std::string::npos || toLower(d.category).find(q) != std::string::npos || toLower(d.language).find(q) != std::string::npos) {
            out.push_back(d);
        }
    }
    return out;
}

const ScriptComponentDef& ScriptCatalog::get(const std::string& name) const {
    for (const auto& d : defs_) {
        if (d.name == name) return d;
    }
    throw std::runtime_error("Unknown script component: " + name);
}

PropertiesManager::PropertiesManager(ScriptCatalog& catalog) : catalog_(catalog) {}

std::string PropertiesManager::inspect(const Entity& entity) const {
    std::ostringstream out;
    out << "entity=" << entity.entityId;
    for (const auto& [compName, props] : entity.components) {
        out << "\n  [" << compName << "]";
        for (const auto& [k, v] : props) {
            out << "\n    " << k << "=" << v;
        }
    }
    return out.str();
}

void PropertiesManager::addScript(Entity& entity, const std::string& scriptName) {
    const auto& def = catalog_.get(scriptName);
    auto& slot = entity.components[def.name];
    for (const auto& prop : def.properties) {
        slot[prop.name] = prop.defaultValue;
    }
}

void PropertiesManager::setProperty(Entity& entity, const std::string& scriptName, const std::string& propName, const std::string& value) {
    auto it = entity.components.find(scriptName);
    if (it == entity.components.end()) {
        throw std::runtime_error("Entity missing script: " + scriptName);
    }
    it->second[propName] = value;
}

bool AssetPipeline::importAsset(const std::string& source, std::string& outError) {
    const std::unordered_set<std::string> supported = {".fbx", ".gltf", ".obj", ".png", ".tga", ".tiff", ".wav", ".ogg"};
    const auto ext = toLower(std::filesystem::path(source).extension().string());
    if (!supported.contains(ext)) {
        outError = "Unsupported asset type: " + ext;
        return false;
    }
    assets_.push_back({source, ext});
    return true;
}

bool AssetPipeline::exportBundle(const std::string& outputPath, std::string& outError) const {
    std::filesystem::create_directories(std::filesystem::path(outputPath).parent_path());
    std::ofstream out(outputPath);
    if (!out) {
        outError = "Failed to open output: " + outputPath;
        return false;
    }
    out << "{\n  \"assets\": [\n";
    for (size_t i = 0; i < assets_.size(); ++i) {
        out << "    {\"source\": \"" << assets_[i].first << "\", \"type\": \"" << assets_[i].second << "\"}";
        if (i + 1 < assets_.size()) out << ",";
        out << "\n";
    }
    out << "  ]\n}\n";
    return true;
}

MountResolver::MountResolver(std::vector<std::string> mountOrder) : mountOrder_(std::move(mountOrder)) {}

std::string MountResolver::resolve(const std::string& assetRelPath,
                                   const std::unordered_map<std::string, std::unordered_set<std::string>>& mountedAssets) const {
    for (auto it = mountOrder_.rbegin(); it != mountOrder_.rend(); ++it) {
        const auto foundMount = mountedAssets.find(*it);
        if (foundMount != mountedAssets.end() && foundMount->second.contains(assetRelPath)) {
            return *it + "/" + assetRelPath;
        }
    }
    return "";
}

EditorApp::EditorApp() : properties_(catalog_) {
    entities_.emplace("door_01", Entity{"door_01"});
    entities_.emplace("switch_01", Entity{"switch_01"});
    entities_.emplace("npc_01", Entity{"npc_01"});
}

void EditorApp::loadDefaultScripts() {
    catalog_.registerCppComponent(ScriptComponentDef{
        "HealthCpp", "C++", "gameplay", {ScriptProperty{"MaxHealth", "int", "100"}, ScriptProperty{"Regen", "bool", "false"}}
    });
    catalog_.registerCppComponent(ScriptComponentDef{
        "MoverCpp", "C++", "movement", {ScriptProperty{"Speed", "float", "4.5"}}
    });

    const std::vector<std::string> candidates = {
        "games/game_alpha/scripts/DoorTrigger.cs",
        "../games/game_alpha/scripts/DoorTrigger.cs",
        "games/game_alpha/scripts_cs/DoorTrigger.cs",
        "../games/game_alpha/scripts_cs/DoorTrigger.cs",
    };
    bool loaded = false;
    for (const auto& c : candidates) {
        if (std::filesystem::exists(c)) {
            catalog_.loadCSharpComponentFile(c);
            loaded = true;
            break;
        }
    }
    if (!loaded) {
        throw std::runtime_error("Could not locate DoorTrigger.cs in known script paths");
    }
}

std::string EditorApp::handle(const std::string& commandLine) {
    const auto parts = split(commandLine);
    if (parts.empty()) return "";

    const auto& cmd = parts[0];
    if (cmd == "entities") {
        std::ostringstream out;
        for (const auto& [id, _] : entities_) out << id << " ";
        return trim(out.str());
    }
    if (cmd == "select") {
        selected_ = &entities_.at(parts.at(1));
        return "selected " + selected_->entityId + "\n" + properties_.inspect(*selected_);
    }
    if (cmd == "inspect") {
        return properties_.inspect(requireSelection());
    }
    if (cmd == "scripts") {
        const auto q = parts.size() > 1 ? parts[1] : "";
        const auto defs = catalog_.search(q);
        std::ostringstream out;
        for (const auto& d : defs) out << d.name << "(" << d.language << ") ";
        return trim(out.str());
    }
    if (cmd == "addscript") {
        properties_.addScript(requireSelection(), parts.at(1));
        return properties_.inspect(requireSelection());
    }
    if (cmd == "set") {
        properties_.setProperty(requireSelection(), parts.at(1), parts.at(2), parts.at(3));
        return properties_.inspect(requireSelection());
    }
    if (cmd == "import") {
        std::string err;
        if (!pipeline_.importAsset(parts.at(1), err)) return "error: " + err;
        return "imported " + parts.at(1);
    }
    if (cmd == "export") {
        std::string err;
        if (!pipeline_.exportBundle(parts.at(1), err)) return "error: " + err;
        return "exported " + parts.at(1);
    }

    return "error: unknown command";
}

Entity& EditorApp::requireSelection() {
    if (!selected_) throw std::runtime_error("No entity selected");
    return *selected_;
}

std::vector<std::string> EditorApp::split(const std::string& s) {
    std::istringstream in(s);
    std::vector<std::string> out;
    std::string token;
    while (in >> token) out.push_back(token);
    return out;
}

} // namespace rcae

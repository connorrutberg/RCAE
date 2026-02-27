#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace rcae {

struct ScriptProperty {
    std::string name;
    std::string type;
    std::string defaultValue;
};

struct ScriptComponentDef {
    std::string name;
    std::string language; // C++ or C#
    std::string category;
    std::vector<ScriptProperty> properties;
};

struct Entity {
    std::string entityId;
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> components;
};

class ScriptCatalog {
public:
    void registerCppComponent(const ScriptComponentDef& def);
    void loadCSharpComponentFile(const std::string& path);
    std::vector<ScriptComponentDef> search(const std::string& query) const;
    const ScriptComponentDef& get(const std::string& name) const;

private:
    std::vector<ScriptComponentDef> defs_;
};

class PropertiesManager {
public:
    explicit PropertiesManager(ScriptCatalog& catalog);
    std::string inspect(const Entity& entity) const;
    void addScript(Entity& entity, const std::string& scriptName);
    void setProperty(Entity& entity, const std::string& scriptName, const std::string& propName, const std::string& value);

private:
    ScriptCatalog& catalog_;
};

class AssetPipeline {
public:
    bool importAsset(const std::string& source, std::string& outError);
    bool exportBundle(const std::string& outputPath, std::string& outError) const;
    const std::vector<std::pair<std::string, std::string>>& assets() const { return assets_; }

private:
    std::vector<std::pair<std::string, std::string>> assets_; // source, extension
};

class MountResolver {
public:
    explicit MountResolver(std::vector<std::string> mountOrder);
    std::string resolve(const std::string& assetRelPath,
                        const std::unordered_map<std::string, std::unordered_set<std::string>>& mountedAssets) const;

private:
    std::vector<std::string> mountOrder_;
};

class EditorApp {
public:
    EditorApp();

    std::string handle(const std::string& commandLine);
    void loadDefaultScripts();

private:
    Entity& requireSelection();
    static std::vector<std::string> split(const std::string& s);

    ScriptCatalog catalog_;
    PropertiesManager properties_;
    AssetPipeline pipeline_;
    std::unordered_map<std::string, Entity> entities_;
    Entity* selected_ {nullptr};
};

} // namespace rcae

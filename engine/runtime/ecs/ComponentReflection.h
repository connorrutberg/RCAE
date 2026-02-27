#pragma once

#include <string>
#include <vector>

namespace rcae {

struct PropertyDesc {
    std::string name;
    std::string type;   // float, bool, vec3, asset_ref, etc.
    std::string category;
};

struct ComponentDesc {
    std::string name;
    std::vector<PropertyDesc> properties;
};

class IComponentRegistry {
public:
    virtual ~IComponentRegistry() = default;
    virtual const std::vector<ComponentDesc>& GetAll() const = 0;
};

} // namespace rcae

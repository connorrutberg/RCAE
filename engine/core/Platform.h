// Platform abstraction layer
#pragma once

class Platform {
public:
    virtual void Init() = 0;
    virtual void Shutdown() = 0;
};

// Platform abstraction layer
#ifndef PLATFORM_H
#define PLATFORM_H

class Platform {
public:
    virtual void Initialize() = 0;
    // Other platform-specific methods
};

#endif // PLATFORM_H

#pragma once

// Interface for objects/services that require explicit initialization
class IInitializable {
public:
    virtual ~IInitializable() = default;

    // Initialize the service (e.g., load resources, set up dependencies)
    virtual void initialize() = 0;
};

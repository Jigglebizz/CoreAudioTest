#pragma once

#include <typeindex>
#include <unordered_map>
#include <memory>

#include "services/AbstractService.h"
/**
 * What's your context?
 * This is how I'm handling depencency injection
 */
class Context {
private:
    // Unordered map for speed - will be doing lots of lookups
    // Maps what is essentially a class type to 
    // an associated unique_ptr.
    // I don't really use any fancy unique_ptr features here,
    // I just want to communicate ownership and take advantage of RAII.
    // This will help keep initialize/teardown simple
    std::unordered_map< std::type_index, std::unique_ptr<AbstractService>> mServices;
public:
    // Very basic template programming. The function provides a 
    // simple-to-use interface and enforces strong typing
    // of services, as opposed to looking up a void* or other more
    // C-style technique
    template<typename T>
    T* const GetService() noexcept {
        return dynamic_cast<T*>(mServices.at(typeid(T)).get());
    }

    // No "Add Service" function is intentional.
    // This function enforces that all services have the 
    // same constructor, which should ensure a guaranteed
    // atomic/modular set of services. 
    // 
    template<typename T>
    void InitializeService() {
        mServices[typeid(T)] = std::make_unique<T>( *this);
    }
};
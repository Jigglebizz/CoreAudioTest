#pragma once

#include <typeindex>
#include <unordered_map>
#include <memory>

#include "services/AbstractService.h"

class Context {
private:
    std::unordered_map< std::type_index, std::unique_ptr<AbstractService>> mServices;
public:
    template<typename T>
    T* const GetService() noexcept {
        return dynamic_cast<T*>(mServices.at(typeid(T)).get());
    }

    template<typename T>
    void InitializeService() {
        mServices[typeid(T)] = std::make_unique<T>( *this);
    }
};
#pragma once

namespace engine {

class Entity;

class Component {
public:
    Entity* entity;
    
    Component() : entity(nullptr) {}
    virtual ~Component() = default;
};

}
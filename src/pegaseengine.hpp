#pragma once

#include <iostream>
#include <vector>

#include "vector3.hpp"
#include "rigidbody.hpp"
#include "collider.hpp"

// ==================== Class for engine ====================
class PegaseEngine {
public:
    std::vector<std::unique_ptr<RigidBody>> bodies;
    Vec3 gravity;
    
    PegaseEngine() : gravity(0, -9.81, 0) {}
    
    RigidBody* addBody(const Vec3& pos, double mass, std::shared_ptr<CollisionShape> shape) {
        auto body = std::make_unique<RigidBody>(pos, mass, shape);
        RigidBody* ptr = body.get();
        bodies.push_back(std::move(body));
        return ptr;
    }
    
    void step(double dt);
    void printState();
};

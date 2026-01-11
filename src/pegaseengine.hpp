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
    
    void step(double dt) {
        // Appliquer la gravité
        for (auto& body : bodies) {
            if (!body->isStatic) {
                body->applyForce(gravity * body->mass);
            }
        }
        
        // Intégrer les mouvements
        for (auto& body : bodies) {
            body->integrate(dt);
        }
        
        // Détecter et résoudre les collisions
        for (size_t i = 0; i < bodies.size(); ++i) {
            for (size_t j = i + 1; j < bodies.size(); ++j) {
                CollisionInfo info;
                if (Collider::checkCollision(bodies[i].get(), bodies[j].get(), info)) {
                    Collider::resolveCollision(info);
                }
            }
        }
    }
    
    void printState() {
        std::cout << "\n=== État du système ===" << std::endl;
        for (size_t i = 0; i < bodies.size(); ++i) {
            std::cout << "Corps " << i << ": pos = ";
            bodies[i]->position.print();
            std::cout << ", vel = ";
            bodies[i]->velocity.print();
            std::cout << std::endl;
        }
    }
};

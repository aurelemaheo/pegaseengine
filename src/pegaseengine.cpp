#include <iostream>
#include <vector>
#include <cmath>
#include <memory>
#include <algorithm>

#include "vector3.hpp"
#include "rigidbody.hpp"
#include "collider.hpp"

// ==================== Moteur physique ====================
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

// ==================== Exemple d'utilisation ====================
int main() {
    PegaseEngine engine;
    
    std::cout << "=== Moteur Physique 3D ===" << std::endl;
    std::cout << "Simulation de corps rigides avec gravité et collisions\n" << std::endl;
    
    // Créer un sol (plan statique)
    auto ground = engine.addBody(Vec3(0, 0, 0), 0, 
                                 std::make_shared<PlaneShape>(Vec3(0, 1, 0), 0));
    ground->makeStatic();
    
    // Créer des sphères qui tombent
    auto sphere1 = engine.addBody(Vec3(0, 10, 0), 1.0, 
                                  std::make_shared<SphereShape>(1.0));
    sphere1->restitution = 0.7;
    
    auto sphere2 = engine.addBody(Vec3(2.5, 15, 0), 1.5, 
                                  std::make_shared<SphereShape>(0.8));
    sphere2->restitution = 0.8;
    
    // Créer une boîte
    auto box = engine.addBody(Vec3(-3, 8, 0), 2.0, 
                              std::make_shared<BoxShape>(Vec3(1, 1, 1)));
    box->restitution = 0.5;
    box->friction = 0.5;
    
    // Simulation
    double timeStep = 0.016;  // ~60 FPS
    double totalTime = 0.0;
    int steps = 200;
    
    std::cout << "Début de la simulation..." << std::endl;
    
    for (int i = 0; i < steps; ++i) {
        engine.step(timeStep);
        totalTime += timeStep;
        
        // Afficher l'état tous les 30 pas
        if (i % 30 == 0) {
            std::cout << "\nTemps: " << totalTime << "s";
            engine.printState();
        }
    }
    
    std::cout << "\n\n=== Simulation terminée ===" << std::endl;
    std::cout << "Temps total simulé: " << totalTime << " secondes" << std::endl;
    
    return 0;
}

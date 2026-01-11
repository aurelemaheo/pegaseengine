
#include <iostream>
#include <vector>
#include <cmath>
#include <memory>
#include <algorithm>

#include "vector3.hpp"
#include "rigidbody.hpp"
#include "collider.hpp"
#include "pegaseengine.hpp"

// Use Case 1
int main() {
    PegaseEngine engine;
    
    std::cout << "=== Moteur Physique 3D ===" << std::endl;
    std::cout << "Simulation de corps rigides avec gravité et collisions\n" << std::endl;
    
    // Create static plane
    auto ground = engine.addBody(Vec3(0, 0, 0), 0, 
                                 std::make_shared<PlaneShape>(Vec3(0, 1, 0), 0));
    ground->makeStatic();
    
    // Create spheres
    auto sphere1 = engine.addBody(Vec3(0, 10, 0), 1.0, 
                                  std::make_shared<SphereShape>(1.0));
    sphere1->restitution = 0.7;
    
    auto sphere2 = engine.addBody(Vec3(2.5, 15, 0), 1.5, 
                                  std::make_shared<SphereShape>(0.8));
    sphere2->restitution = 0.8;
    
    // Create box
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

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
   
    RandomBodyConfig config;
    engine.addRandomBodies(config);

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
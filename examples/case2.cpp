
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

    std::cout << "=== 3D Physics Engine ===" << std::endl;
    std::cout << "Simulation of rigid bodies with gravity and collisions\n" << std::endl;
   
    RandomBodyConfig config;
    engine.addRandomBodies(config);

    // Simulation
    double timeStep = 0.016;  // ~60 FPS
    double totalTime = 0.0;
    int steps = 200;
    
    std::cout << "Starting simulation..." << std::endl;
    
    unsigned long startSimTime = std::chrono::duration_cast<std::chrono::microseconds>
        (std::chrono::system_clock::now().time_since_epoch()).count();
    
    for (int i = 0; i < steps; ++i) {
        engine.step(timeStep);
        totalTime += timeStep;
        
        // Afficher l'état tous les 30 pas
        if (i % 30 == 0) {
            std::cout << "\nTime: " << totalTime << "s";
            //engine.printState();
        }
    }

    unsigned long endSimTime = std::chrono::duration_cast<std::chrono::microseconds>
        (std::chrono::system_clock::now().time_since_epoch()).count();

    double simDuration = (endSimTime - startSimTime) / 1e6; // Convert to seconds
    
    std::cout << "\n\n=== Simulation completed ===" << std::endl;
    std::cout << "Total simulated time: " << simDuration << " seconds" << std::endl;
    
    return 0;
}
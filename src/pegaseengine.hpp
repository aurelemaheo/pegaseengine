#pragma once

#include <iostream>
#include <vector>

#include "vector3.hpp"
#include "rigidbody.hpp"
#include "collider.hpp"

struct RandomBodyConfig
{
    // Plages pour le nombre d'objets
    int minCount = 5;
    int maxCount = 100;
    
    // Plages pour la position
    Vec3 positionMin = Vec3(-10, 5, -10);
    Vec3 positionMax = Vec3(10, 30, 10);
    
    // Plages pour la vitesse initiale
    Vec3 velocityMin = Vec3(-5, -2, -5);
    Vec3 velocityMax = Vec3(5, 5, 5);
    
    // Plages pour la masse
    double massMin = 0.5;
    double massMax = 5.0;
    
    // Plages pour la taille des formes
    double sizeMin = 0.5;
    double sizeMax = 2.0;
    
    // Plages pour les propriétés physiques
    double restitutionMin = 0.3;
    double restitutionMax = 0.9;
    
    double frictionMin = 0.1;
    double frictionMax = 0.8;
    
    // Types de formes autorisées (si vide, toutes sont autorisées)
    std::vector<ShapeType> allowedShapes;
    
    // Seed pour la reproductibilité (0 = aléatoire basé sur le temps)
    unsigned int seed = 0;
};

// ==================== Class for engine ====================
class PegaseEngine {
public:
    std::vector<std::unique_ptr<RigidBody>> bodies;
    Vec3 gravity;
    
    PegaseEngine() : gravity(0, -9.81, 0) {}

    // Add a rigid body to the simulation
    RigidBody* addBody(const Vec3& pos, double mass, std::shared_ptr<CollisionShape> shape);
    
    // Generate a random number of bodies for simulation 
    void addRandomBodies(const RandomBodyConfig& config);

    void step(double dt);
    void printState();
};

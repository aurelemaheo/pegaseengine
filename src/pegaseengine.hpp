#pragma once

#include <iostream>
#include <vector>

#include "vector3.hpp"
#include "rigidbody.hpp"
#include "collider.hpp"

struct RandomBodyConfig
{
    // Range defining number of bodies to create
    int minCount = 2000;
    int maxCount = 4000;

    // Range for body positions
    Vec3 positionMin = Vec3(-10, 5, -10);
    Vec3 positionMax = Vec3(10, 30, 10);

    // Range for initial velocity
    Vec3 velocityMin = Vec3(-5, -2, -5);
    Vec3 velocityMax = Vec3(5, 5, 5);

    // Range for mass
    double massMin = 0.5;
    double massMax = 5.0;
    
    // Range for shape sizes
    double sizeMin = 0.5;
    double sizeMax = 2.0;
    
    // Range for physical properties
    double restitutionMin = 0.3;
    double restitutionMax = 0.9;
    
    double frictionMin = 0.1;
    double frictionMax = 0.8;

    // Allowed shapes
    std::vector<ShapeType> allowedShapes;
    
    // Seed for reproducibility (0 = random based on time)
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

#pragma once

#include <iostream>
#include <vector>
#include <thread>
#include <future>
#include <set>

#include "vector3.hpp"
#include "rigidbody.hpp"
#include "collider.hpp"
#include "pegaseengine.hpp"

template<typename Func>
void ParallelFor(size_t start, size_t end, Func f) {

  size_t range = end - start;
  unsigned int num_threads = std::thread::hardware_concurrency();
  size_t ckSize = (end - start) / num_threads;

  std::vector<std::future<void>> futures;

    for(unsigned int t = 0; t < num_threads; ++t) {
        size_t chunk_start = start + t * ckSize;
        size_t chunk_end = (t == num_threads - 1) ? end : chunk_start + ckSize;
    
        futures.push_back(std::async(std::launch::async, [chunk_start, chunk_end, &f]() {
        for(size_t i = chunk_start; i < chunk_end; ++i) {
            f(i);
        }
        }));
    }  
}

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
    Vec3 gravity;
    
    PegaseEngine() : gravity(0, -9.81, 0) {}

    // Add a rigid body to the simulation
    RigidBody* addBody(const Vec3& pos, double mass, std::shared_ptr<CollisionShape> shape);
    
    // Generate a random number of bodies for simulation 
    void addRandomBodies(const RandomBodyConfig& config);
    // Broad phase stage: parse space tree and find potential collisions
    void BroadPhase();
    // Narrow phase stage: check potential collisions and resolve them
    void NarrowPhase();

    void UpdateObjectAABB();

    // Run the simulation
    void run(int numSteps, double dt);
    //void step(double dt);
    void printState();

  private:

     std::set<Collider::CollisionInfo> broadphaseCollisions;
     std::unique_ptr<Octree<RigidBody*>> octree;
     std::vector<RigidBody*> bodies;

     void step(double dt);
     void basicStep(double dt);
     void broadPhaseStep(double dt);
     void prepareBroadPhase();
     void execBroadPhase();
     void execNarrowPhase();

};

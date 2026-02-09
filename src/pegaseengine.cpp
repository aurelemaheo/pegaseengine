#include <iostream>
#include <vector>
#include <cmath>
#include <memory>
#include <algorithm>
#include <chrono>
#include <random>
#include <set>
#include <list>

#include "vector3.hpp"
#include "rigidbody.hpp"
#include "octree.hpp"
#include "collider.hpp"
#include "pegaseengine.hpp"

#define PAR 1

// Add a rigid body to the simulation
//RigidBody* PegaseEngine::addBody(const Vec3& pos, double mass, std::shared_ptr<CollisionShape> shape) 
//{
//        auto body = std::make_unique<RigidBody>(pos, mass, shape);
//        RigidBody* ptr = body.get();
//        bodies.push_back(body);
//        return ptr;
//}

// Generate a random number of bodies for simulation
void PegaseEngine::addRandomBodies(const RandomBodyConfig& config)
{
  unsigned int actualseed = config.seed;

  if(actualseed == 0) {
    actualseed = std::chrono::system_clock::now().time_since_epoch().count();
  }

  std::mt19937 rng(actualseed);

  // Distributions
  std::uniform_int_distribution<int> countDist(config.minCount, config.maxCount);
  std::uniform_real_distribution<double> posXDist(config.positionMin.x, config.positionMax.x);
  std::uniform_real_distribution<double> posYDist(config.positionMin.y, config.positionMax.y);
  std::uniform_real_distribution<double> posZDist(config.positionMin.z, config.positionMax.z);
  std::uniform_real_distribution<double> velXDist(config.velocityMin.x, config.velocityMax.x);
  std::uniform_real_distribution<double> velYDist(config.velocityMin.y, config.velocityMax.y);
  std::uniform_real_distribution<double> velZDist(config.velocityMin.z, config.velocityMax.z);
  std::uniform_real_distribution<double> massDist(config.massMin, config.massMax);
  std::uniform_real_distribution<double> sizeDist(config.sizeMin, config.sizeMax);
  std::uniform_real_distribution<double> restitutionDist(config.restitutionMin, config.restitutionMax);
  std::uniform_real_distribution<double> frictionDist(config.frictionMin, config.frictionMax);

  std::vector<ShapeType> shapes = config.allowedShapes;

  if(shapes.empty()) {
    shapes = {ShapeType::SPHERE, ShapeType::BOX};
  }

  std::uniform_int_distribution<size_t> shapeDist(0, shapes.size() - 1);

  int bodyCount = countDist(rng);

  for(int i = 0; i < bodyCount; ++i) {
    Vec3 position(posXDist(rng), posYDist(rng), posZDist(rng));
    Vec3 velocity(velXDist(rng), velYDist(rng), velZDist(rng));
    double mass = massDist(rng);
    double restitution = restitutionDist(rng);
    double friction = frictionDist(rng);

    ShapeType shapeType = shapes[shapeDist(rng)];
    std::shared_ptr<CollisionShape> shape;

    if(shapeType == ShapeType::SPHERE) {
      double radius = sizeDist(rng);
      shape = std::make_shared<SphereShape>(radius);
    } else if(shapeType == ShapeType::BOX) {
      double halfSize = sizeDist(rng) / 2.0;
      shape = std::make_shared<BoxShape>(Vec3(halfSize, halfSize, halfSize));
    }

    auto body = new RigidBody(position, mass, shape);
    //RigidBody* ptr = body.get();
    bodies.push_back(body);
    //free(ptr);
  }
}

void PegaseEngine::UpdateObjectAABB() 
{
    for (auto& body : bodies) {
        body->UpdateBroadphaseAABB();
    }
}

// Broad phase function
void PegaseEngine::BroadPhase()
{
    broadphaseCollisions.clear();
    Octree<RigidBody*> tree(Vec3(1024,1024,1024),7);
    
    std::vector<RigidBody*>::const_iterator first;
    std::vector<RigidBody*>::const_iterator last;

    //for(auto it = first; it != last; ++it) {
    for(auto& body : bodies) {
        Vec3 halfSizes;
        body->GetBroadphaseAABB(halfSizes);

        Vec3 pos = body->getPosition();
        tree.Insert(body, pos, halfSizes);
    }

    tree.OperateOnContents([&](std::list<OctreeEntry<RigidBody*>>& entries) {
      Collider::CollisionInfo info;
      for(auto it1 = entries.begin(); it1 != entries.end(); it1++) {
        for(auto it2 = std::next(it1); it2 != entries.end(); it2++) {
          info.bodyA = it1->object;
          info.bodyB = it2->object;
          broadphaseCollisions.insert(info);
        }
      }

    });

}

// Narrow phase function
void PegaseEngine::NarrowPhase() 
{
  for(std::set<Collider::CollisionInfo>::iterator it = broadphaseCollisions.begin(); 
    it != broadphaseCollisions.end(); ++it) {
    Collider::CollisionInfo info = *it;
      if (Collider::checkCollision(info.bodyA, info.bodyB, info)) {
          Collider::resolveCollision(info);
      }
  }

}

void PegaseEngine::basicStep(double dt) 
{

  // Appliquer la gravité
  for (auto& body : bodies) {
    if (!body->isStatic) {
      body->applyForce(gravity * body->mass);
    }
  }

  unsigned long timeStep1 = std::chrono::duration_cast<std::chrono::microseconds>
        (std::chrono::system_clock::now().time_since_epoch()).count();

#if PAR == 1
  // Parallel version

  ParallelFor(0, bodies.size(), [&](size_t i) {
    bodies[i]->integrate(dt);
  });

#else      
  // Sequential version
  for (auto& body : bodies) {
    body->integrate(dt);
  }
 #endif // PAR

  unsigned long timeStep2 = std::chrono::duration_cast<std::chrono::microseconds>
        (std::chrono::system_clock::now().time_since_epoch()).count();

  // Detect and solve collisions
#if PAR == 1
  // Parallel version
  ParallelFor(0, bodies.size(), [&](size_t i) {
    for (size_t j = i + 1; j < bodies.size(); ++j) {
      Collider::CollisionInfo info;
      if (Collider::checkCollision(bodies[i], bodies[j], info)) {
        Collider::resolveCollision(info);
      }
    }
  });

#else
  // Sequential version
  for (size_t i = 0; i < bodies.size(); ++i) {
    for (size_t j = i + 1; j < bodies.size(); ++j) {
      CollisionInfo info;
      if (Collider::checkCollision(bodies[i].get(), bodies[j].get(), info)) {
        Collider::resolveCollision(info);
      }
    }
  }

#endif // PAR

unsigned long timeStep3 = std::chrono::duration_cast<std::chrono::microseconds>
  (std::chrono::system_clock::now().time_since_epoch()).count();

std::cout << "Integration time: " << (timeStep2 - timeStep1) / 1e3 << " ms, ";
std::cout << "Collision time: " << (timeStep3 - timeStep2) / 1e3 << " ms" << std::endl;
 
}

void PegaseEngine::broadPhaseStep(double dt) 
{
    BroadPhase();
    NarrowPhase();
}

void PegaseEngine::step(double dt)
{ 

  // Appliquer la gravité
  for (auto& body : bodies) {
    if (!body->isStatic) {
      body->applyForce(gravity * body->mass);
    }
  }

  
  unsigned long timeStep1 = std::chrono::duration_cast<std::chrono::microseconds>
        (std::chrono::system_clock::now().time_since_epoch()).count();


#if PAR == 1
  // Parallel version

  ParallelFor(0, bodies.size(), [&](size_t i) {
    bodies[i]->integrate(dt);
  });

#else      
  // Sequential version
  for (auto& body : bodies) {
    body->integrate(dt);
  }
 #endif // PAR

  unsigned long timeStep2 = std::chrono::duration_cast<std::chrono::microseconds>
        (std::chrono::system_clock::now().time_since_epoch()).count();

  // Detect and solve collisions
#if PAR == 1
  // Parallel version
  ParallelFor(0, bodies.size(), [&](size_t i) {
    for (size_t j = i + 1; j < bodies.size(); ++j) {
      Collider::CollisionInfo info;
      if (Collider::checkCollision(bodies[i], bodies[j], info)) {
        Collider::resolveCollision(info);
      }
    }
  });

#else
  // Sequential version
  for (size_t i = 0; i < bodies.size(); ++i) {
    for (size_t j = i + 1; j < bodies.size(); ++j) {
      CollisionInfo info;
      if (Collider::checkCollision(bodies[i].get(), bodies[j].get(), info)) {
        Collider::resolveCollision(info);
      }
    }
  }

#endif // PAR

unsigned long timeStep3 = std::chrono::duration_cast<std::chrono::microseconds>
  (std::chrono::system_clock::now().time_since_epoch()).count();

std::cout << "Integration time: " << (timeStep2 - timeStep1) / 1e3 << " ms, ";
std::cout << "Collision time: " << (timeStep3 - timeStep2) / 1e3 << " ms" << std::endl;
  
}

void PegaseEngine::run(int numSteps, double dt) 
{
    // Main simulation loop (not implemented here)
    for(int i=0 ; i<numSteps ; ++i) {
        step(dt);
    }
}

void PegaseEngine::printState() 
{
    std::cout << "\n=== System state ===" << std::endl;
    for (size_t i = 0; i < bodies.size(); ++i) {
        std::cout << "Body " << i << ": pos = ";
        bodies[i]->position.print();
        std::cout << ", vel = ";
        bodies[i]->velocity.print();
        std::cout << std::endl;
    }
}

#include <iostream>
#include <vector>
#include <cmath>
#include <memory>
#include <algorithm>
#include <chrono>
#include <random>
//#include <omp.h>

#include "vector3.hpp"
#include "rigidbody.hpp"
#include "collider.hpp"
#include "pegaseengine.hpp"


// Add a rigid body to the simulation
RigidBody* PegaseEngine::addBody(const Vec3& pos, double mass, std::shared_ptr<CollisionShape> shape) 
{
        auto body = std::make_unique<RigidBody>(pos, mass, shape);
        RigidBody* ptr = body.get();
        bodies.push_back(std::move(body));
        return ptr;
}

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

    auto body = std::make_unique<RigidBody>(position, mass, shape);
    //RigidBody* ptr = body.get();
    bodies.push_back(std::move(body));
    //free(ptr);
  }
}

void PegaseEngine::step(double dt)
{ 

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
  //#pragma omp parallel for schedule(dynamic)
  for (size_t i = 0; i < bodies.size(); ++i) {
    for (size_t j = i + 1; j < bodies.size(); ++j) {
      CollisionInfo info;
      if (Collider::checkCollision(bodies[i].get(), bodies[j].get(), info)) {
        Collider::resolveCollision(info);
      }
    }
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

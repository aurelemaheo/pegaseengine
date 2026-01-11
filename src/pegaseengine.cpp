#include <iostream>
#include <vector>
#include <cmath>
#include <memory>
#include <algorithm>

#include "vector3.hpp"
#include "rigidbody.hpp"
#include "collider.hpp"
#include "pegaseengine.hpp"

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
  for (size_t i = 0; i < bodies.size(); ++i) {
    for (size_t j = i + 1; j < bodies.size(); ++j) {
      CollisionInfo info;
      if (Collider::checkCollision(bodies[i].get(), bodies[j].get(), info)) {
        Collider::resolveCollision(info);
      }
    }
  }

}

void PegaseEngine::printState() {
    std::cout << "\n=== État du système ===" << std::endl;
    for (size_t i = 0; i < bodies.size(); ++i) {
        std::cout << "Corps " << i << ": pos = ";
        bodies[i]->position.print();
        std::cout << ", vel = ";
        bodies[i]->velocity.print();
        std::cout << std::endl;
    }
}

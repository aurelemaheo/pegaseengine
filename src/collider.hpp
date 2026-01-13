#pragma once

#include <iostream>

#include "vector3.hpp"
#include "rigidbody.hpp"

// ==================== Collision Information ====================
struct CollisionInfo {
    RigidBody* bodyA;
    RigidBody* bodyB;
    Vec3 normal;        // Normal de collision (de A vers B)
    double penetration; // Profondeur de pénétration
    Vec3 contactPoint;  // Point de contact

    CollisionInfo() : bodyA(nullptr), bodyB(nullptr), penetration(0) {}
};

class Collider
{
  public:
    static bool checkCollision(RigidBody* a, RigidBody* b, CollisionInfo& info);
     static void resolveCollision(CollisionInfo& info);

  private:
    static bool sphereVsSphere(RigidBody* a, RigidBody* b, CollisionInfo& info);
    static bool sphereVsPlane(RigidBody* sphere, RigidBody* plane, CollisionInfo& info);
    static bool boxVsBox(RigidBody* a, RigidBody* b, CollisionInfo& info);
    static void correctPosition(CollisionInfo& info);
    static void applyFriction(CollisionInfo& info, const Vec3& relativeVelocity, double normalImpulse);

};

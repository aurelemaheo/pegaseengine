#pragma once

#include <iostream>

#include "vector3.hpp"
#include "rigidbody.hpp"

// Class handling collision detection and resolution between rigid bodies
class Collider
{
  public:

    // ==================== Collision Information ====================
    struct CollisionInfo {
      RigidBody* bodyA;
      RigidBody* bodyB;
      Vec3 normal;        // Normal de collision (de A vers B)
      double penetration; // Profondeur de pénétration
      Vec3 contactPoint;  // Point de contact

      CollisionInfo() : bodyA(nullptr), bodyB(nullptr), penetration(0) {}

      bool operator < ( const CollisionInfo & other ) const {
        size_t otherHash = ( size_t ) other . bodyA + (( size_t ) other . bodyB << 8);
        size_t thisHash = ( size_t ) bodyA + (( size_t ) bodyB << 8);

        return ( thisHash < otherHash );
      }
    };



    static bool checkCollision(RigidBody* a, RigidBody* b, CollisionInfo& info);
    static void resolveCollision(CollisionInfo& info);
    static bool AABBTest(const Vec3& posA, const Vec3& posB,
                         const Vec3& sizeA, const Vec3& sizeB) {
        float halfA_x = sizeA.x * 0.5f, halfA_y = sizeA.y * 0.5f, halfA_z = sizeA.z * 0.5f;
        float halfB_x = sizeB.x * 0.5f, halfB_y = sizeB.y * 0.5f, halfB_z = sizeB.z * 0.5f;

        return std::abs(posA.x - posB.x) < (halfA_x + halfB_x) &&
               std::abs(posA.y - posB.y) < (halfA_y + halfB_y) &&
               std::abs(posA.z - posB.z) < (halfA_z + halfB_z);
    }

  private:
    static bool sphereVsSphere(RigidBody* a, RigidBody* b, CollisionInfo& info);
    static bool sphereVsPlane(RigidBody* sphere, RigidBody* plane, CollisionInfo& info);
    static bool boxVsBox(RigidBody* a, RigidBody* b, CollisionInfo& info);
    static void correctPosition(CollisionInfo& info);
    static void applyFriction(CollisionInfo& info, const Vec3& relativeVelocity, double normalImpulse);

};

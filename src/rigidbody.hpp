#pragma once

#include <iostream>

#include "vector3.hpp"

// Collision shape
enum class ShapeType {
    AABB,
    OOB,
    SPHERE,
    BOX,
    MESH,
    PLANE
};

struct CollisionShape {
    ShapeType type;
    virtual ~CollisionShape() = default;
};

struct SphereShape : public CollisionShape {
    double radius;
    SphereShape(double r) : radius(r) { type = ShapeType::SPHERE; }
};

struct BoxShape : public CollisionShape {
    Vec3 halfExtents;  // Semi dimensions (width/2, height/2, depth/2)
    BoxShape(const Vec3& extents) : halfExtents(extents) { type = ShapeType::BOX; }
};

struct PlaneShape : public CollisionShape {
    Vec3 normal;
    double distance;  // Distance from origin
    PlaneShape(const Vec3& n, double d) : normal(n.normalized()), distance(d) {
        type = ShapeType::PLANE;
    }
};


/* 
 * Rigid body : position, velocity, acceleration, force 
 * @brief  Class representing a rigid body in the physics engine, with properties and methods for physics simulation
 */

class RigidBody {
public:
    Vec3 position;
    Vec3 velocity;
    Vec3 acceleration;
    Vec3 force;
    
    double mass;
    double inverseMass;
    double restitution;  // Bouncing coefficient  (0 = inélastique, 1 = élastique parfait)
    double friction;     // Friction coefficient
    
    std::shared_ptr<CollisionShape> shape;
    
    bool isStatic;  // Static bodies do not move
    
    RigidBody(const Vec3& pos, double m, std::shared_ptr<CollisionShape> s)
        : position(pos), velocity(0, 0, 0), acceleration(0, 0, 0), force(0, 0, 0),
          mass(m), restitution(0.6), friction(0.3), shape(s), isStatic(false) {
        inverseMass = (m > 0) ? 1.0 / m : 0.0;
    }

    bool GetBroadphaseAABB(Vec3& outSize) {
        if (shape->type == ShapeType::SPHERE) {
            SphereShape* sphere = static_cast<SphereShape*>(shape.get());
            outSize = Vec3(sphere->radius, sphere->radius, sphere->radius);
            return true;
        } else if (shape->type == ShapeType::BOX) {
            BoxShape* box = static_cast<BoxShape*>(shape.get());
            outSize = box->halfExtents;
            return true;
        }
        return false; // AABB not supported for this shape type
    }

    void UpdateBroadphaseAABB() {

        if(shape->type == ShapeType::AABB) {
            // AABB for a sphere
            // Pas besoin de stocker séparément, on peut calculer à la volée
            broadphaseAABB = GetHalfDimensions(); 
        } else if (shape->type == ShapeType::SPHERE) {
            SphereShape* sphere = static_cast<SphereShape*>(shape.get());
            broadphaseAABB = Vec3(sphere->radius, sphere->radius, sphere->radius);
        }
        else if(shape->type == ShapeType::BOX) {
            BoxShape* box = static_cast<BoxShape*>(shape.get());
            // AABB for a box : center = position, semi dimensions = halfExtents
            broadphaseAABB = box->halfExtents;
        }

    }
    
    void applyForce(const Vec3& f) {
        if (!isStatic) {
            force += f;
        }
    }
    
    void applyImpulse(const Vec3& impulse) {
        if (!isStatic) {
            velocity += impulse * inverseMass;
        }
    }
    
    void integrate(double dt) {
        if (isStatic) return;
        
        // Integrate Verlet semi-implicit
        acceleration = force * inverseMass;
        velocity += acceleration * dt;
        position += velocity * dt;
        
        // Reset forces
        force = Vec3(0, 0, 0);
    }
    
    void makeStatic() {
        isStatic = true;
        inverseMass = 0.0;
        velocity = Vec3(0, 0, 0);
    }

    Vec3 GetHalfDimensions() {
        return halfSizes;
    }
    
    Vec3 getPosition() const { return position; }

private:
    Vec3 halfSizes; // Store semi dimensions for AABB and BOX shapes
    Vec3 broadphaseAABB; // Store the half dimensions of the AABB for broadphase detection

};


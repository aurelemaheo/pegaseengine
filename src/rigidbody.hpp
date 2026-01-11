#pragma once

#include <iostream>

#include "vector3.hpp"

// ==================== Forme de collision ====================
enum class ShapeType {
    SPHERE,
    BOX,
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
    Vec3 halfExtents;  // Demi-dimensions (largeur/2, hauteur/2, profondeur/2)
    BoxShape(const Vec3& extents) : halfExtents(extents) { type = ShapeType::BOX; }
};

struct PlaneShape : public CollisionShape {
    Vec3 normal;
    double distance;  // Distance de l'origine
    PlaneShape(const Vec3& n, double d) : normal(n.normalized()), distance(d) {
        type = ShapeType::PLANE;
    }
};


// ==================== Corps rigide ====================
class RigidBody {
public:
    Vec3 position;
    Vec3 velocity;
    Vec3 acceleration;
    Vec3 force;
    
    double mass;
    double inverseMass;
    double restitution;  // Coefficient de rebond (0 = inélastique, 1 = élastique parfait)
    double friction;     // Coefficient de friction
    
    std::shared_ptr<CollisionShape> shape;
    
    bool isStatic;  // Les objets statiques ne bougent pas
    
    RigidBody(const Vec3& pos, double m, std::shared_ptr<CollisionShape> s)
        : position(pos), velocity(0, 0, 0), acceleration(0, 0, 0), force(0, 0, 0),
          mass(m), restitution(0.6), friction(0.3), shape(s), isStatic(false) {
        inverseMass = (m > 0) ? 1.0 / m : 0.0;
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
        
        // Intégration de Verlet semi-implicite
        acceleration = force * inverseMass;
        velocity += acceleration * dt;
        position += velocity * dt;
        
        // Réinitialiser les forces
        force = Vec3(0, 0, 0);
    }
    
    void makeStatic() {
        isStatic = true;
        inverseMass = 0.0;
        velocity = Vec3(0, 0, 0);
    }
};


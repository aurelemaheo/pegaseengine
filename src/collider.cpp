#include "collider.hpp"

// Check collision between each pair of bodies and body shape type
bool Collider::checkCollision(RigidBody* a, RigidBody* b, CollisionInfo& info) 
{
        info.bodyA = a;
        info.bodyB = b;

        // Sphere-Sphere
        if (a->shape->type == ShapeType::SPHERE && b->shape->type == ShapeType::SPHERE) {
            return sphereVsSphere(a, b, info);
        }

        // Sphere-Plane
        if (a->shape->type == ShapeType::SPHERE && b->shape->type == ShapeType::PLANE) {
            return sphereVsPlane(a, b, info);
        }
        if (b->shape->type == ShapeType::SPHERE && a->shape->type == ShapeType::PLANE) {
            bool result = sphereVsPlane(b, a, info);
            if (result) {
                info.normal = info.normal * -1.0;
                std::swap(info.bodyA, info.bodyB);
            }
            return result;
        }

        // Box-Box (AABB simplifié)
        if (a->shape->type == ShapeType::BOX && b->shape->type == ShapeType::BOX) {
            return boxVsBox(a, b, info);
        }

        return false;
    }

    void Collider::resolveCollision(CollisionInfo& info) 
    {
        RigidBody* a = info.bodyA;
        RigidBody* b = info.bodyB;

        // Correction de position (projection)
        correctPosition(info);

        // Calculer la vitesse relative
        Vec3 relativeVelocity = b->velocity - a->velocity;
        double velocityAlongNormal = relativeVelocity.dot(info.normal);

        // Ne résoudre que si les objets se rapprochent
        if (velocityAlongNormal > 0) return;

        // Calculer le coefficient de restitution
        double restitution = std::min(a->restitution, b->restitution);

        // Calculer l'impulsion
        double impulseScalar = -(1.0 + restitution) * velocityAlongNormal;
        impulseScalar /= (a->inverseMass + b->inverseMass);

        Vec3 impulse = info.normal * impulseScalar;

        // Appliquer l'impulsion
        a->applyImpulse(impulse * -1.0);
        b->applyImpulse(impulse);

        // Friction
        applyFriction(info, relativeVelocity, impulseScalar);
    }

    bool Collider::sphereVsSphere(RigidBody* a, RigidBody* b, CollisionInfo& info) 
    {
        auto sphereA = std::static_pointer_cast<SphereShape>(a->shape);
        auto sphereB = std::static_pointer_cast<SphereShape>(b->shape);

        Vec3 delta = b->position - a->position;
        double distSquared = delta.lengthSquared();
        double radiusSum = sphereA->radius + sphereB->radius;

        if (distSquared < radiusSum * radiusSum) {
            double dist = std::sqrt(distSquared);
            info.normal = dist > 0 ? delta / dist : Vec3(0, 1, 0);
            info.penetration = radiusSum - dist;
            info.contactPoint = a->position + info.normal * sphereA->radius;
            return true;
        }
        return false;
    }

    bool Collider::sphereVsPlane(RigidBody* sphere, RigidBody* plane, CollisionInfo& info) 
    {
        auto sphereShape = std::static_pointer_cast<SphereShape>(sphere->shape);
        auto planeShape = std::static_pointer_cast<PlaneShape>(plane->shape);

        double dist = sphere->position.dot(planeShape->normal) - planeShape->distance;

        if (dist < sphereShape->radius) {
            info.normal = planeShape->normal;
            info.penetration = sphereShape->radius - dist;
            info.contactPoint = sphere->position - planeShape->normal * dist;
            return true;
        }
        return false;
    }

    bool Collider::boxVsBox(RigidBody* a, RigidBody* b, CollisionInfo& info) 
    {
        auto boxA = std::static_pointer_cast<BoxShape>(a->shape);
        auto boxB = std::static_pointer_cast<BoxShape>(b->shape);

        // Test AABB simple (Axis-Aligned Bounding Box)
        Vec3 minA = a->position - boxA->halfExtents;
        Vec3 maxA = a->position + boxA->halfExtents;
        Vec3 minB = b->position - boxB->halfExtents;
        Vec3 maxB = b->position + boxB->halfExtents;

        // Test de chevauchement sur chaque axe
        bool overlapX = maxA.x >= minB.x && maxB.x >= minA.x;
        bool overlapY = maxA.y >= minB.y && maxB.y >= minA.y;
        bool overlapZ = maxA.z >= minB.z && maxB.z >= minA.z;

        if (overlapX && overlapY && overlapZ) {
            // Calculer la pénétration sur chaque axe
            double penX = std::min(maxA.x - minB.x, maxB.x - minA.x);
            double penY = std::min(maxA.y - minB.y, maxB.y - minA.y);
            double penZ = std::min(maxA.z - minB.z, maxB.z - minA.z);

            // Trouver l'axe de pénétration minimale
            if (penX < penY && penX < penZ) {
                info.penetration = penX;
                info.normal = (b->position.x > a->position.x) ? Vec3(1, 0, 0) : Vec3(-1, 0, 0);
            } else if (penY < penZ) {
                info.penetration = penY;
                info.normal = (b->position.y > a->position.y) ? Vec3(0, 1, 0) : Vec3(0, -1, 0);
            } else {
                info.penetration = penZ;
                info.normal = (b->position.z > a->position.z) ? Vec3(0, 0, 1) : Vec3(0, 0, -1);
            }

            info.contactPoint = (a->position + b->position) * 0.5;
            return true;
        }
        return false;
    }

    void Collider::correctPosition(CollisionInfo& info) 
    {
        const double percent = 0.8;  // Pourcentage de correction
        const double slop = 0.01;    // Seuil de pénétration autorisée

        double correctionAmount = std::max(info.penetration - slop, 0.0) /
                                  (info.bodyA->inverseMass + info.bodyB->inverseMass) * percent;

        Vec3 correction = info.normal * correctionAmount;

        if (!info.bodyA->isStatic) {
            info.bodyA->position -= correction * info.bodyA->inverseMass;
        }
        if (!info.bodyB->isStatic) {
            info.bodyB->position += correction * info.bodyB->inverseMass;
        }
    }

    void Collider::applyFriction(CollisionInfo& info, const Vec3& relativeVelocity, double normalImpulse) 
    {
        RigidBody* a = info.bodyA;
        RigidBody* b = info.bodyB;

        // Calculer la vitesse tangentielle
        Vec3 tangent = relativeVelocity - info.normal * relativeVelocity.dot(info.normal);
        double tangentLength = tangent.length();

        if (tangentLength > 0.0001) {
            tangent = tangent / tangentLength;

            // Coefficient de friction
            double friction = std::sqrt(a->friction * b->friction);

            // Impulsion de friction (loi de Coulomb)
            double frictionImpulse = -tangentLength / (a->inverseMass + b->inverseMass);
            frictionImpulse = std::max(-std::abs(normalImpulse) * friction,
                                       std::min(frictionImpulse, std::abs(normalImpulse) * friction));

            Vec3 frictionVector = tangent * frictionImpulse;

            a->applyImpulse(frictionVector * -1.0);
            b->applyImpulse(frictionVector);
        }
    }
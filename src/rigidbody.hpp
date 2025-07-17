#include <iostream>

#include "basebody.hpp"
#include "vector3.hpp"

class RigidBody : public BaseBody
{
  public:
   RigidBody(Vector3 pos, double mass): _pos(pos), _vel(0, 0, 0), _acc(0, 0, 0), _mass(mass) {}
   void setVelocity(Vector3 v) {} 

  private:
    Vector3 _pos;
    Vector3 _vel;
    Vector3 _acc;
    double _mass;
};

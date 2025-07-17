#include <iostream>

#include "pegaseengine.hpp"
#include "rigidbody.hpp"

int main()
{

  PegaseEngine engine(0.016);

  RigidBody ball1(Vector3(0, 15, 0), 1.0);
  RigidBody ball2(Vector3(-10, 20, 0), 2.0);
  RigidBody ball3(Vector3(10, 25, 0), 0.5);

  ball1.setVelocity(Vector3(5, 0, 0));
  ball2.setVelocity(Vector3(8, 2, 0));
  ball3.setVelocity(Vector3(-3, 5, 0));

  engine.addBody(ball1);
  engine.addBody(ball2);
  engine.addBody(ball3);

  return 0;
}

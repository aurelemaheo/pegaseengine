#include <iostream>
#include <vector>

#include "rigidbody.hpp"

class PegaseEngine
{
  public:
   PegaseEngine(double dt) : timeStep(dt) {}
   ~PegaseEngine();

   void addBody(RigidBody body)
   {
     bodies.push_back(body);
   }

   void run();
   int destroy();

  private:
    std::vector<RigidBody> bodies;
    double timeStep;
};

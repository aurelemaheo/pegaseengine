#include <iostream>
#include <vector>

#include "rigidbody.hpp"

class PegaseEngine
{
  public:
   PegaseEngine();
   ~PegaseEngine();

   void addBody(RigidBody body)
   {
     bodies.push_back(body);
   }

   void update()
   {
    
   }

   int destroy();

  private:
    std::vector<RigidBody> bodies;
};

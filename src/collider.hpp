#ifndef COLLIDER_H
#define COLLIDER_H

class Collider
{

  Collider(){}
  ~Collider(){}


  // Methods for detecting collisions
  void TestSpherePlaneCollision();

  // Methods for handling collisions
  void HandleSpherePlaneCollision();
};

#endif // COLLIDER_H

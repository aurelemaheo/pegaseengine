#ifndef SOLID_H
#define SOLID_H

/*
Base class defining a solid object
*/
class Solid
{
  public:

    //virtual void setForce(float newton, int x, int y, int z) = 0;

    enum solid_type
    {
      SPHERE,
      CUBE,
      JOINT
    };

    struct Pos
    {
      float _x;
      float _y;
      float _z;
    };
  
    struct lVelocity
    {
      float _x;
      float _y;
      float _z;
    };

  private:
    int   	_Id;          	       // Object identifier
    float 	_weight;               // Weight of the solid
    Pos  	_curr_pos; 	       // Position (x,y,z) of the solid at t time
    lVelocity   _initial_speed;        // Linear velocity (x,y,z)
    float 	_ang_speed;            // Angular velocity (rad/sec) 
    bool 	is_colliding;          // Boolean notifying whether solid is colliding with another object or not
  
};

#endif //SOLID_H

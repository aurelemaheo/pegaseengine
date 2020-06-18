#ifndef BODY_H
#define BODY_H

#include "maths/vector3.hpp"

/**
* Base class defining a body
*/
class Body
{
  public:

    //virtual void setForce(float newton, int x, int y, int z) = 0;
    //Solid(int Id):_Id(Id){}
    Body(){_curr_pos = NULL; _lin_velocity = NULL;}
    ~Body(){}

    enum body_type
    {
      STATIC,
      DYNAMIC
    };

    struct Pos
    {
      double _x;
      double _y;
      double _z;
    };
  
    struct lVelocity
    {
      double _x;
      double _y;
      double _z;
    };
    
    void setWeight(double w){_weight = w;}
    void setPosition(Vector3<double> pos){_curr_pos = pos;}
    void setPosition(double x, double y, double z){_curr_pos._x = x; _curr_pos._y = y; _curr_pos._z = z;}
    Vector3<double> getPosition(){return _curr_pos;}
    void setLinVelocity(double x, double y, double z){_lin_velocity._x = x; _lin_velocity._y = y; _lin_velocity._z = z;}
    void setLinVelocity(Vector3<double> linVel){_lin_velocity = linVel;}
    Vector3<double> getLinVelocity(){return _lin_velocity;}
    void setAngVelocity(double angvel){_ang_velocity = angvel;}

  protected:
    int   	            _Id;          	       // Object identifier
    double 	            _weight;               // Weight of the solid
    Vector3<double>  	_curr_pos; 	           // Position (x,y,z) of the solid at t time
    Vector3<double>     _lin_velocity;         // Linear velocity (x,y,z)
    double 	            _ang_velocity;         // Angular velocity (rad/sec)
    bool 	            is_colliding;          // Boolean notifying whether solid is colliding with another object or not
    body_type           _type;

  private:
  
};

#endif //BODY_H

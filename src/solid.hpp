#ifndef SOLID_H
#define SOLID_H

/*
Base class defining a solid object
*/
class Solid
{
  public:

    virtual void setForce(float newton, int x, int y, int z) = 0;

    struct Pos
    {
      float _x;
      float _y;
      float _z;
    };

  private:

    float _weight;  
    Pos  _curr_pos;        // Position (x,y,z) of the solid at t time
    float _initial_speed;
    float _ang_speed;      // Angular velocity (rad/sec) 
  
};

#endif SOLID_H

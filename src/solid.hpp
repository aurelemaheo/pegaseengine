#ifndef SOLID_H
#define SOLID_H

struct Pos
{
 float _x;
 float _y;
 float _z;
};

/*
Base class defining a solid object
*/
class Solid
{
public:

 virtual void setForce(float newton, int x, int y, int z) = 0;

private:
 float _weight;  
 Pos  _curr_pos;
 float _initial_speed;
 float _ang_speed;
  
};

#endif SOLID_H

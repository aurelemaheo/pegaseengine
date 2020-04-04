#ifndef OBJECT_H
#define OBJECT_H

struct Pos
{
 float _x;
 float _y;
 float _z;
};


class base_object
{
public:

 void setForce(float newton, int x, int y, int z);

private:
 float _weight;  
 Pos  _curr_pos;
 float _initial_speed;
 float _ang_speed;
  
};

#endif //OBJECT_H

#ifndef OBJECT_H
#define OBJECT_H

struct pos_s
{
 float _x;
 float _y;
 float _z;
};

typedef pos_s pos_t;

class base_object
{
public:

 void setForce(float newton, int x, int y, int z);

private:
 float _weight;  
 pos_t _curr_pos;
 float _speed;
 float _ang_speed;
  
};

#endif //OBJECT_H

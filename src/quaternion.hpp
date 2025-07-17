#include <iostream>


class Quaternion
{
  public: 
    Quaternion();
    Quaternion(double x, double y, double z, double w);

  private:
    double _x;
    double _y;
    double _z;
    double _w;
};

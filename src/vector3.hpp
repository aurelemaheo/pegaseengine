#include <iostream>

class Vector3
{
  public:

    Vector3(double _x = 0, double _y = 0, double _z = 0):_x(_x), _y(_y), _z(_z) {}

    Vector3 operator+(const Vector3& other) const
    {
      return Vector3(_x + other._x, _y + other._y, _z + other._y);
    } 
   
    Vector3 operator*(Vector3& other) const
    {
      return Vector3(_x * other._x, _y * other._y, _z * other._z);
    }  

    Vector3 operator*(double scalar) const
    {
      return Vector3(_x * scalar, _y * scalar, _z * scalar);
    }

  private:

    double _x;
    double _y;
    double _z;

};

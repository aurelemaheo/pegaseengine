#ifndef QUATERNION_H
#define QUATERNION_H

#include <iostream>
#include <cmath>

template <typename T>
class Quaternion
{
  public:
    Quaternion();
    Quaternion(T x, T y, T z, T w);
    ~Quaternion();
    T length()
    {
      return std::sqrt(_x*_x + _y*_y + _z*_z + _w*_w);
    }

    void normalize();
    T dot(const Quaternion&);
    void BuildQuaternionFromEulerAngles(T anglex, T angley, T anglez);
    Quaternion operator*(const Quaternion& quaternion)
    {
      return Quaternion(_w * quaternion._x + quaternion._w * _x + _y * quaternion._z - _z * quaternion._y,
                    _w * quaternion._y + quaternion._w * _y + _z * quaternion._x - _x * quaternion._z,
                    _w * quaternion._z + quaternion._w * _z + _x * quaternion._y - _y * quaternion._x,
                    _w * quaternion._w - quaternion._w * _x + _y * quaternion._x - _z * quaternion._z);
    }

    T _x;
    T _y;
    T _z;
    T _w;

  private:
 
};

#endif //QUATERNION_H

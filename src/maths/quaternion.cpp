#include <iostream>

#include "quaternion.hpp"

template <typename T>
void Quaternion<T>::BuildQuaternionFromEulerAngles(T anglex, T angley, T anglez)
{
  T angle = anglex * T(0.5);
  const T sinX = std::sin(angle);
  const T cosX = std::cos(angle);

  angle = angley * T(0.5);
  const T sinY = std::sin(angle);
  const T cosY = std::cos(angle);

  angle = anglez * T(0.5);
  const T sinZ = std::sin(angle);
  const T cosZ = std::cos(angle);

  const T cosYcosZ = cosY * cosZ;
  const T sinYcosZ = sinY * cosZ;
  const T cosYsinZ = cosY * sinZ;
  const T sinYsinZ = sinY * sinZ;

  _x = sinX * cosYcosZ - cosX * sinYsinZ;
  _y = cosX * sinYcosZ + sinX * cosYsinZ;
  _z = cosX * cosYsinZ - sinX * sinYcosZ;
  _w = cosX * cosYcosZ + sinX * sinYsinZ;

  normalize();

}

template <typename T>
void Quaternion<T>::normalize()
{
  T len = length();

  assert(len > 0);

  _x /= len;
  _y /= len;
  _z /= len;
  _w /= len;
}

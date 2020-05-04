#ifndef QUATERNION_H
#define QUATERNION_H

template <typename T>
class Quaternion
{
  public:
    Quaternion();
    Quaternion(T x, T y, T z, T w);
    ~Quaternion();
    T length();

  private:
    T _x;
    T _y;
    T _z;
    T _w;
 
};

#endif //QUATERNION_H

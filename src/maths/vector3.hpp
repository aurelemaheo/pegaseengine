#ifndef VECTOR3_H
#define VECTOR3_H

template <typename T>
class Vector3
{

  public:
    Vector3(){}
    Vector3(double x, double y, double z){}  // Constructor
    ~Vector3(){}   			    // Destructor
    float length() const;
    Vector3 dot(Vector3&);		    // Scalar product
    Vector3 max(const Vector3& vector1, const Vector3& vector2);	
    Vector3 min(const Vector3& vector1, const Vector3& vector2);	
 
    T _x; // X component
    T _y; // Y component
    T _z; // Z component
};

#endif //VECTOR3_H

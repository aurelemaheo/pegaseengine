#ifndef VECTOR3_H
#define VECTOR3_H

struct Vector3
{

  public:
    double _x; // X component
    double _y; // Y component
    double _z; // Z component

    Vector3(double x, double y, double z);  // Constructor
    ~Vector3();   			    // Destructor
    float length() const;
    Vector3 dot(vector3&);		    // Scalar product
    Vector3 max(const Vector3& vector1, const Vector3& vector2);	
    Vector3 min(const Vector3& vector1, const Vector3& vector2);	
  private:

};

#endif //VECTOR3_H

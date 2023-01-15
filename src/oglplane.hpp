#ifndef OGLPLANE_H
#define OGLPLANE_H

#include "maths/vector3.hpp"

class OGLPlane : public BaseBody
{
public:
	Plan(Vector3<double> point1, Vector3<double> point2, Vector3<double> point3, Vector3<double> point4);	

	// Accessors	
	Vector3<double> getNormal(){ return _normal; }
       
  	Vector3<double> getPoint1() { return _point1; }
  	Vector3<double> getPoint2() { return _point2; }
  	Vector3<double> getPoint3() { return _point3; }
  	Vector3<double> getPoint4() { return _point4; }

        void 	Display();

private:
	Vector3<double> _point1;
	Vector3<double> _point2;
	Vector3<double> _point3;
	Vector3<double> _point4;

	Vector3<double> _normal;
};

#endif //OGLPLANE_H

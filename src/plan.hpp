#ifndef PLAN_H
#define PLAN_H

#include "maths/vector3.hpp"

class Plan
{
public:
	Plan(Vector3<double> point1, Vector3<double> point2, Vector3<double> point3, Vector3<double> point4);	

	// Accessors	
	Vector3 getNormal(){ return _normal; }
       
  	Vector3 getPoint1() { return _point1; }
  	Vector3 getPoint2() { return _point2; }
  	Vector3 getPoint3() { return _point3; }
  	Vector3 getPoint4() { return _point4; }

        void 	Display();

private:
	Vector3 _point1;
	Vector3 _point2;
	Vector3 _point3;
	Vector3 _point4;

	Vector3 _normal;
};

#endif //PLAN_H

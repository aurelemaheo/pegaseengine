/*
 *  Vector3.cpp
 *  Physic Engine
 *
 *
 */

#include "vector3.hpp"

//Addition
Vector3 operator+ (const Vector3& V1, const Vector3& V2)
{ return Vector3(V1.x+V2.x,V1.y+V2.y,V1.z+V2.z); }
void operator+= (Vector3& V1, const Vector3& V2)
{ V1.x+=V2.x; V1.y+=V2.y ; V1.z+=V2.z; }

//Soustraction
Vector3 operator- (const Vector3& V1, const Vector3& V2)
{ return Vector3(V1.x-V2.x,V1.y-V2.y,V1.z-V2.z); }
void operator-= (Vector3& V1, const Vector3& V2)
{ V1.x-=V2.x; V1.y-=V2.y; V1.z-=V2.z; }

// Multiplication d'un vecteur par un scalaire
Vector3 operator* (const Vector3& V1, const float k)
{ return Vector3(V1.x*k,V1.y*k,V1.z*k); }
Vector3 operator* (const float k, const Vector3& V1)
{ return Vector3(V1.x*k,V1.y*k,V1.z*k); }

// Division d'un vecteur par un scalaire
Vector3 operator/ (const Vector3& V1, const float k)
{ return Vector3(V1.x/k,V1.y/k,V1.z/k); }
Vector3 operator/ (const float k, const Vector3& V1)
{ return Vector3(V1.x/k,V1.y/k,V1.z/k); }

Vector3 operator- (const Vector3& V)
{return Vector3(-V.x,-V.y,-V.z); }

//Comparaison de vecteurs
bool operator==(const Vector3& V1, const Vector3& V2)
{
	if(V1.x==V2.x && V1.y==V2.y && V1.z==V2.z)
		return true;
	return false;
}

bool operator!=(const Vector3& V1, const Vector3& V2)
{
	if(V1.x!=V2.x || V1.y!=V2.y || V1.z!=V2.z)
		return true;
	return false;
}

Vector3 Vector3::Normaliser()
{
	float mag = Module(); 
	
	if ( mag <= 0.001f) 
		return Vector3(0.0f, 0.0f, 0.0f);
	
	x /= mag;
	y /= mag;
	z /= mag;
	
	return Vector3(x,y,z);
}

Vector3 Vector3::Normalised()
{
	float ix = x;
	float iy = y;
	float iz = z;

	float mag = Module(); 
	
	if ( mag <= 0.001f) 
		return Vector3(0.0f, 0.0f, 0.0f);
	
	ix /= mag;
	iy /= mag;
	iz /= mag;
	
	return Vector3(ix,iy,iz);
}

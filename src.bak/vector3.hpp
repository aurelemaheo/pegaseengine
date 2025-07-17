/*
 *  Vector3.h
 *  NewRaytracer
 *
 *  Created by Grégory Corgié on 20/02/05.
 *  Copyright 2005 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef __Vector3_h__
#define __Vector3_h__

#include <math.h>
#include <stdio.h>
#include <iostream>

class Vector3
{
public:
	float x,y,z;
	
	// ---------------------------------------------------------------
	//Constructeur
	// ---------------------------------------------------------------
	Vector3():x(0.0),y(0.0),z(0.0){}
	Vector3(float _x,float _y,float _z):x(_x),y(_y),z(_z){}
	Vector3(const Vector3& A):x(A.x),y(A.y),z(A.z){}
	~Vector3() {}
	
	// ---------------------------------------------------------------
	// Opérations sur les vecteurs
	// ---------------------------------------------------------------

	//Redéfinition des opérateurs de base sur les vecteurs + - * /
	friend Vector3 operator+ (const Vector3& V1, const Vector3& V2);
	friend void operator+=(Vector3& V1, const Vector3& V2);
	friend Vector3 operator-(const Vector3& V1, const Vector3& V2);
	friend void operator-=(Vector3& V1, const Vector3& V2);
	friend bool operator==(const Vector3& V1, const Vector3& V2);
	friend bool operator!=(const Vector3& V1, const Vector3& V2);
	
	friend float operator*(const Vector3& V1, const Vector3& V2) { return (V1.x*V2.x+V1.y*V2.y+V1.z*V2.z); }
	friend Vector3 operator*(const Vector3& V1, const float k);
	friend Vector3 operator*(const float k, const Vector3& V1);
	
	friend Vector3 operator/ (const Vector3& V1,const float k);
	friend Vector3 operator/ (const float k, const Vector3& V1);
	friend Vector3 operator- (const Vector3& V); // Opposé
	
	// Operations propres aux vecteurs
	Vector3 ProdVectoriel (const Vector3& V) {return Vector3( (y*V.z) - (z*V.y), (z*V.x) - (x*V.z), (x*V.y) - (y*V.x)  ) ;}
	
	float Module() const 					{ return (float) sqrt( x*x+ y*y+ z*z); }
	float ModuleCarre() const 				{ return ( x*x+ y*y+ z*z); }
	Vector3 Normaliser();
	Vector3 Normalised();
	void Affichage() 						{ printf("\n%f \n%f \n%f \n",x,y,z);}
	void Set(float _x, float _y, float _z)  { x=_x, y=_y, z=_z; }

	inline void DBG_Vector() { std::cout<< x << " " << y << " " << z << std::endl; } 
};

#endif

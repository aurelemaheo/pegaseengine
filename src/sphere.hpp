/*
 *  Sphere.h
 *  PhysicEngine
 *
 *  Created by Julien Peyre and Grégory Corgié on 17/11/06.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 
 *
 */

#ifndef __SPHERE_h__ 

#define __SPHERE_h__

#include "basebody.hpp"

class Sphere : public BaseBody
{
	
public:
	
	Sphere(float fRayon);
	Sphere(float fRayon, Vector3 Vpos);
	Sphere(int iMass, float fRayon, Vector3 Vpos);
	Sphere(float fRayon, float fXpos, float fYpos, float fZpos);
	

	// ---------------------------------------------------------------
	// Accessors
	// ---------------------------------------------------------------	
	float GetRayon()  { return m_fRayon; }
	
	// ---------------------------------------------------------------
	// Methods
	// ---------------------------------------------------------------
	void Display();
	
private:
		
		// ---------------------------------------------------------------
		// Attributs
		// ---------------------------------------------------------------
		float 		m_fRayon;
	
};

#endif

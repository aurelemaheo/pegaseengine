/*
 *  Plan.h
 *  PhysicEngine
 *
 *  Created by Julien Peyre and Grégory Corgié on 17/11/06.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 
 *
 */

#ifndef __PLAN_h__ 

#define __PLAN_h__

#include "basebody.hpp"

class Plan : public BaseBody
{

public:
	
	Plan(Vector3 Point1, Vector3 Point2, Vector3 Point3);

	// ---------------------------------------------------------------
	// Accessors
	// ---------------------------------------------------------------
	Vector3 GetNormal()  { return m_vNormal; }

	Vector3 GetPoint1()  { return m_vPoint1; }
	Vector3 GetPoint2()  { return m_vPoint2; }
	Vector3 GetPoint3()  { return m_vPoint3; }
	Vector3 GetPoint4()  { return m_vPoint4; }

	// ---------------------------------------------------------------
	// Methods
	// ---------------------------------------------------------------
	void Display();
	
private:
	
	// ---------------------------------------------------------------
	// Attributs
	// ---------------------------------------------------------------

	Vector3		m_vNormal;

	Vector3		m_vPoint1;
	Vector3		m_vPoint2;
	Vector3		m_vPoint3;
	Vector3		m_vPoint4;
	
};

#endif

/*
 *  Plan.cpp
 *  PhysicEngine
 *
 *  Created by Julien Peyre and Grégory Corgié on 17/11/06.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */

#include "config.hpp"

#ifdef __MAC_OS__
#include <GLUT/glut.h>
#endif

#ifdef __linux__
#include <GL/glut.h>
#endif

#ifdef __WINDOWS__
#include <stdlib.h>
#include <GL/glut.h>
#endif

#include "plan.hpp"

// ---------------------------------------------------------------	
// Name : Constructors
// Goal : 
// Params : 
// ---------------------------------------------------------------
Plan::Plan(Vector3 Point1, Vector3 Point2, Vector3 Point3)
{
	m_iType = BODY_PLAN;

	m_vPoint1 = Point1;
	m_vPoint2 = Point2;
	m_vPoint3 = Point3;
	m_vPoint4 = Point1 - Point2 + Point3;

	m_vPos	=	Point1;

	m_vNormal = (Point2-Point1).ProdVectoriel(Point3-Point1);
	m_vNormal = m_vNormal.Normaliser();

}

// ---------------------------------------------------------------	
// Name : Display
// Goal : Display plan with "this" parameters
// Params : None
// ---------------------------------------------------------------	
void Plan::Display()
{	
	if ( !GetMateriau() )
		return;

	glEnable( GL_TEXTURE_2D );
	glBindTexture (GL_TEXTURE_2D, m_pMateriau->GetTexId() );

	if ( m_pMateriau->IsTransparent() )
	{
		glEnable(GL_BLEND); //active le blending
		glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR); // Set coef de blending
	}

	// Color material for lightning
	glEnable(GL_COLOR_MATERIAL);    
	glColorMaterial(GL_FRONT,GL_AMBIENT_AND_DIFFUSE );
	glMaterialfv(GL_FRONT,GL_DIFFUSE, m_pMateriau->GetDiffuse() );
	glMaterialfv(GL_FRONT,GL_SPECULAR, m_pMateriau->GetSpecular() );
	glMateriali(GL_FRONT,GL_SHININESS, 100.f );

	glBegin(GL_QUADS);
	
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(m_vPoint1.x,m_vPoint1.y,m_vPoint1.z);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(m_vPoint2.x,m_vPoint2.y,m_vPoint2.z);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(m_vPoint3.x,m_vPoint3.y,m_vPoint3.z);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(m_vPoint4.x,m_vPoint4.y,m_vPoint4.z);
	
	glEnd();

	glDisable( GL_TEXTURE_2D) ;	
	glDisable( GL_COLOR_MATERIAL) ;
	glDisable( GL_BLEND) ;
}

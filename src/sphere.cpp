/*
 *  Sphere.cpp
 *  PhysicEngine
 *
 *  Created by Julien Peyre and GrŽgory CorgiŽ on 17/11/06.
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

#include "sphere.hpp"

// ---------------------------------------------------------------	
// Name : Constructors
// Goal : 
// Params : 
// ---------------------------------------------------------------	
Sphere::Sphere(float fRayon)
{
	m_iType				= BODY_SPHERE;
	
	m_vInitPos			=	Vector3( 0.f, 0.f, 0.f);
	m_vPos				=	Vector3( 0.f, 0.f, 0.f);
	m_vVelocity			=	Vector3( 0.f, 0.f, 0.f);
	m_vAcceleration		=	Vector3( 0.f, 0.f, 0.f);
	
	m_fRayon			=	fRayon;

	m_bIsImmobile		=	false;
}

Sphere::Sphere(float fRayon, float fXpos, float fYpos, float fZpos)
{
	m_iType = BODY_SPHERE;

	m_vInitPos			=	Vector3( fXpos, fYpos, fZpos);
	m_vPos				=	m_vInitPos;
	
	m_vVelocity			=	Vector3( 0.f, 0.f, 0.f);
	m_vAcceleration		=	Vector3( 0.f, 0.f, 0.f);
	
	m_fRayon			=	fRayon;
	
	m_bIsImmobile		=	false;
}

Sphere::Sphere(float fRayon, Vector3 Vpos)
{
	m_iType				=	BODY_SPHERE;
	
	m_vInitPos			=	Vpos;
	m_vPos				=	Vpos;

	m_vVelocity			=	Vector3( 0.f, 0.f, 0.f);
	m_vAcceleration		=	Vector3( 0.f, 0.f, 0.f);
	
	m_fRayon			=	fRayon;

	m_bIsImmobile		=	false;
}

Sphere::Sphere(int iMass, float fRayon, Vector3 Vpos)
{
	m_iType				= BODY_SPHERE;
	
	m_vInitPos			= Vpos;
	m_vPos				= Vpos;
	m_vVelocity			= Vector3( 0.f, 0.f, 0.f);
	m_vAcceleration		= Vector3( 0.f, 0.f, 0.f);
	
	m_fRayon			= fRayon;
	m_fMass				= iMass;

	m_bIsImmobile		=	false;
}
// ---------------------------------------------------------------	
// Name : Display
// Goal : Display sphere with "this" parameters
// Params : None
// ---------------------------------------------------------------	
void Sphere::Display()
{	
	if ( !GetMateriau() )
		return;
	
	glEnable( GL_TEXTURE_2D );
	glBindTexture (GL_TEXTURE_2D, GetMateriau()->GetTexId() );

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

	glPushMatrix();	

	GLUquadric* params = gluNewQuadric(); //création du quadrique
    
	gluQuadricTexture(params,GL_TRUE); //activation des coordonnées de texture

	glTranslatef( m_vPos.x, m_vPos.y, m_vPos.z);	
	gluSphere( params, m_fRayon, 30, 30);
	
	glPopMatrix();
	
	gluDeleteQuadric(params); //je supprime le quadrique

	glDisable( GL_TEXTURE_2D) ;	
	glDisable( GL_COLOR_MATERIAL) ;
	glDisable( GL_BLEND) ;	
}

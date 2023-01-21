/*
 *  Plan.cpp
 *  PhysicEngine
 *
 *  Created by Julien Peyre and Grégory Corgié on 10/01/07.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */

#include "config.hpp"

#ifdef __MAC_OS__
#include <GL/glut.h>
#endif

#ifdef __linux__
#include <GL/glut.h>
#endif

#ifdef __WINDOWS__
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>
#endif

#include "box.hpp"

// ---------------------------------------------------------------	
// Name : Constructors
// Goal : 
// Params : 
// ---------------------------------------------------------------
Box::Box(Vector3 Centre, float dim1, float dim2, float dim3, Vector3 Orientation1, Vector3 Orientation2, float Mass)
{
	float CoeffInertie[9];

	m_iType	= BODY_BOX;

	m_fDim1 = dim1; m_fDim2 = dim2; m_fDim3 = dim3;

	// Construction de la base locale
	m_vR1 = Orientation1;
	m_vR1.Normaliser();
	
	if(fabs(Orientation1 * Orientation2) > 0.05)
	{
		m_vR2.x=-m_vR1.y; m_vR2.y=m_vR1.x; m_vR2.z=0;
	}
	else
		m_vR2 = Orientation2;
	
	m_vR2.Normaliser();
	m_vR3 = m_vR1.ProdVectoriel(m_vR2);
	m_vR3.Normaliser();

	m_vWAng				=	Vector3( 0.0f, 0.f, 0.f );

	m_fMass				=	Mass;

	m_vPos				=	Centre;
	m_vInitPos			=	Centre;
	m_vVelocity			=	Vector3( 0.f, 0.f, 0.f);
	m_vAcceleration		=	Vector3( 0.f, 0.f, 0.f);

	m_bIsImmobile		=	false;

	for(int i=0; i<9; i++)
		CoeffInertie[i]=0.0f;
	CoeffInertie[0] = m_fMass*(m_fDim2*m_fDim2+m_fDim3*m_fDim3)/12;
	CoeffInertie[4] = m_fMass*(m_fDim1*m_fDim1+m_fDim3*m_fDim3)/12;
	CoeffInertie[8] = m_fMass*(m_fDim1*m_fDim1+m_fDim2*m_fDim2)/12;

	m_mInertie.resize(3,3);
	m_mInertieInv.resize(3,3);
	m_mInertie.in(CoeffInertie);
	m_mInertieInv = m_mInertie;
	m_mInertieInv.reverse();

	//m_vWAng.Set(5.f,5.f,0.f);

}

// ---------------------------------------------------------------	
// Name : Display
// Goal : Display plan with "this" parameters
// Params : None
// ---------------------------------------------------------------	
void Box::Display()
{
	Vector3 Point;
	
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

	glBegin(GL_QUADS);

	// Right Face
	// -------------------------------------------------------------------------------------------
	Point = m_vPos + m_vR1*(m_fDim1 * 0.5f) + m_vR2*(m_fDim2 * 0.5f) + m_vR3*(m_fDim3 * 0.5f);
	glTexCoord2f(1.0f, 0.0f); 	
	glVertex3f(Point.x,Point.y,Point.z);
	Point = m_vPos + m_vR1*(m_fDim1 * 0.5f) + m_vR2*(m_fDim2 * 0.5f) - m_vR3*(m_fDim3 * 0.5f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(Point.x,Point.y,Point.z);
	Point = m_vPos + m_vR1*(m_fDim1 * 0.5f) - m_vR2*(m_fDim2 * 0.5f) - m_vR3*(m_fDim3 * 0.5f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(Point.x,Point.y,Point.z);
	Point = m_vPos + m_vR1*(m_fDim1 * 0.5f) - m_vR2*(m_fDim2 * 0.5f) + m_vR3*(m_fDim3 * 0.5f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(Point.x,Point.y,Point.z);

	// Front Face
	// -------------------------------------------------------------------------------------------
	Point = m_vPos + m_vR1*(m_fDim1 * 0.5f) + m_vR2*(m_fDim2 * 0.5f) - m_vR3*(m_fDim3 * 0.5f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(Point.x,Point.y,Point.z);
	Point = m_vPos + m_vR1*(m_fDim1 * 0.5f) - m_vR2*(m_fDim2 * 0.5f) - m_vR3*(m_fDim3 * 0.5f);
	glTexCoord2f(1.0f, 0.0f);	
	glVertex3f(Point.x,Point.y,Point.z);
	Point = m_vPos - m_vR1*(m_fDim1 * 0.5f) - m_vR2*(m_fDim2 * 0.5f) - m_vR3*(m_fDim3 * 0.5f);
	glTexCoord2f(1.0f, 1.0f);	
	glVertex3f(Point.x,Point.y,Point.z);
	Point = m_vPos - m_vR1*(m_fDim1 * 0.5f) + m_vR2*(m_fDim2 * 0.5f) - m_vR3*(m_fDim3 * 0.5f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(Point.x,Point.y,Point.z);

	// Left Face
	// -------------------------------------------------------------------------------------------
	Point = m_vPos - m_vR1*(m_fDim1 * 0.5f) - m_vR2*(m_fDim2 * 0.5f) - m_vR3*(m_fDim3 * 0.5f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(Point.x,Point.y,Point.z);
	Point = m_vPos - m_vR1*(m_fDim1 * 0.5f) + m_vR2*(m_fDim2 * 0.5f) - m_vR3*(m_fDim3 * 0.5f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(Point.x,Point.y,Point.z);
	Point = m_vPos - m_vR1*(m_fDim1 * 0.5f) + m_vR2*(m_fDim2 * 0.5f) + m_vR3*(m_fDim3 * 0.5f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(Point.x,Point.y,Point.z);
	Point = m_vPos - m_vR1*(m_fDim1 * 0.5f) - m_vR2*(m_fDim2 * 0.5f) + m_vR3*(m_fDim3 * 0.5f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(Point.x,Point.y,Point.z);

	// Back Face
	// -------------------------------------------------------------------------------------------
	Point = m_vPos - m_vR1*(m_fDim1 * 0.5f) + m_vR2*(m_fDim2 * 0.5f) + m_vR3*(m_fDim3 * 0.5f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(Point.x,Point.y,Point.z);
	Point = m_vPos + m_vR1*(m_fDim1 * 0.5f) + m_vR2*(m_fDim2 * 0.5f) + m_vR3*(m_fDim3 * 0.5f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(Point.x,Point.y,Point.z);
	Point = m_vPos + m_vR1*(m_fDim1 * 0.5f) - m_vR2*(m_fDim2 * 0.5f) + m_vR3*(m_fDim3 * 0.5f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(Point.x,Point.y,Point.z);
	Point = m_vPos - m_vR1*(m_fDim1 * 0.5f) - m_vR2*(m_fDim2 * 0.5f) + m_vR3*(m_fDim3 * 0.5f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(Point.x,Point.y,Point.z);

	// Top Face
	// -------------------------------------------------------------------------------------------
	Point = m_vPos + m_vR1*(m_fDim1 * 0.5f) + m_vR2*(m_fDim2 * 0.5f) + m_vR3*(m_fDim3 * 0.5f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(Point.x,Point.y,Point.z);
	Point = m_vPos + m_vR1*(m_fDim1 * 0.5f) + m_vR2*(m_fDim2 * 0.5f) - m_vR3*(m_fDim3 * 0.5f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(Point.x,Point.y,Point.z);
	Point = m_vPos - m_vR1*(m_fDim1 * 0.5f) + m_vR2*(m_fDim2 * 0.5f) - m_vR3*(m_fDim3 * 0.5f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(Point.x,Point.y,Point.z);
	Point = m_vPos - m_vR1*(m_fDim1 * 0.5f) + m_vR2*(m_fDim2 * 0.5f) + m_vR3*(m_fDim3 * 0.5f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(Point.x,Point.y,Point.z);

	// Top Face
	// -------------------------------------------------------------------------------------------
	Point = m_vPos + m_vR1*(m_fDim1 * 0.5f) - m_vR2*(m_fDim2 * 0.5f) + m_vR3*(m_fDim3 * 0.5f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(Point.x,Point.y,Point.z);
	Point = m_vPos + m_vR1*(m_fDim1 * 0.5f) - m_vR2*(m_fDim2 * 0.5f) - m_vR3*(m_fDim3 * 0.5f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(Point.x,Point.y,Point.z);
	Point = m_vPos - m_vR1*(m_fDim1 * 0.5f) - m_vR2*(m_fDim2 * 0.5f) - m_vR3*(m_fDim3 * 0.5f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(Point.x,Point.y,Point.z);
	Point = m_vPos - m_vR1*(m_fDim1 * 0.5f) - m_vR2*(m_fDim2 * 0.5f) + m_vR3*(m_fDim3 * 0.5f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(Point.x,Point.y,Point.z);
	
	glEnd();

	glDisable( GL_TEXTURE_2D) ;	
	glDisable( GL_COLOR_MATERIAL) ;
	glDisable( GL_BLEND) ;	
}

// ------------------------------------------------------------------------------	
// Name : UpdateRotation 
// Goal : Update les angles de rotations en fonction de leur vitesse angulaire
// Params : - _dt : Temps ÈcoulÈ depuis le dernier appel
// -------------------------------------------------------------------------------
void Box::UpdateRotation( )
{

	if(IsInCollision()==true)
		ApplyRotation( m_fColAngle, -m_vWAng );
	else
		ApplyRotation( m_fAngle, m_vWAng );

}

void Box::ApplyRotation(float fAngle, Vector3 AxeRotation)
{
	float			fRotCoef[16];
	float			x, y, z;
	TMatrix<float>	m44Rot(4,4);
	Vector3			vRotAxe = AxeRotation;

	vRotAxe.Normaliser();

	x		=	vRotAxe.x;
	y		=	vRotAxe.y;
	z		=	vRotAxe.z;

	fRotCoef[0] = x * x * ( 1 - cosf(fAngle) ) + cosf(fAngle);
	fRotCoef[1] = x * y * ( 1 - cosf(fAngle) ) - z * sinf(fAngle);
	fRotCoef[2] = x * z * ( 1 - cosf(fAngle) ) + y * sinf(fAngle);
	fRotCoef[3] = 0;
	fRotCoef[4] = x * y * ( 1 - cosf(fAngle) ) + z * sinf(fAngle);
	fRotCoef[5] = y * y * ( 1 - cosf(fAngle) ) + cosf(fAngle);
	fRotCoef[6] = y * z * ( 1 - cosf(fAngle) ) - x * sinf(fAngle);
	fRotCoef[7] = 0;
	fRotCoef[8] = x * z * ( 1 - cosf(fAngle) ) - y * sinf(fAngle);
	fRotCoef[9] = y * z * ( 1 - cosf(fAngle) ) + x * sinf(fAngle);
	fRotCoef[10] = z * z * ( 1 - cosf(fAngle) ) + cosf(fAngle);
	fRotCoef[11] = 0;
	fRotCoef[12] = 0;
	fRotCoef[13] = 0;
	fRotCoef[14] = 0;
	fRotCoef[15] = 1;


	m44Rot.in( fRotCoef );

	// Rotation du repere
	m_vR1	= 	m44Rot	*	m_vR1;
	m_vR2	= 	m44Rot	*	m_vR2;
	m_vR3	= 	m44Rot	*	m_vR3;

}

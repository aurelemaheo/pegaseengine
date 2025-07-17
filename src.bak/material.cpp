/*
 *  Materiau.cpp
 *  PhysicEngine
 *
 *  Created by Gr√©gory Corgi√© on 17/11/06.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */

#include "material.hpp"

// ---------------------------------------------------------------	
// Name : Constructor
// Goal : Constructor
// Params : None
// ---------------------------------------------------------------	
Materiau::Materiau()
{
	m_fAmortCoef = 0.7f;
	m_fFrottementGlissementCoef = 0.4f;
	
	m_pAmbient[0] = 0.15f; m_pAmbient[1] = 0.15f; m_pAmbient[2] = 0.15f; m_pAmbient[3] = 1.0f;
	m_pDiffuse[0] = 0.5f; m_pDiffuse[1] = 0.5f; m_pDiffuse[2] = 0.5f; m_pDiffuse[3] = 1.0f;
	m_pSpecular[0] = 0.5f; m_pSpecular[1] = 0.5f; m_pSpecular[2] = 0.5f; m_pSpecular[3] = 1.0f;

	m_bTransp	=	false;
	m_iTexId		=	0;
		
}

Materiau::Materiau( int _TexId, float _fFrot, float _fGliss )
{
	m_iTexId = _TexId; 

	m_fAmortCoef = _fFrot;
	m_fFrottementGlissementCoef = _fGliss;
	
	m_pAmbient[0] = 0.15f; m_pAmbient[1] = 0.15f; m_pAmbient[2] = 0.15f; m_pAmbient[3] = 1.0f;
	m_pDiffuse[0] = 0.5f; m_pDiffuse[1] = 0.5f; m_pDiffuse[2] = 0.5f; m_pDiffuse[3] = 1.0f;
	m_pSpecular[0] = 0.5f; m_pSpecular[1] = 0.5f; m_pSpecular[2] = 0.5f; m_pSpecular[3] = 1.0f;

	m_bTransp	=	false;
}

Materiau::Materiau( float fAmortCoef)
{
	m_fAmortCoef = fAmortCoef;
	m_fFrottementGlissementCoef = 0.4f;
	
	m_pAmbient[0] = 0.15f; m_pAmbient[1] = 0.15f; m_pAmbient[2] = 0.15f; m_pAmbient[3] = 1.0f;
	m_pDiffuse[0] = 0.5f; m_pDiffuse[1] = 0.5f; m_pDiffuse[2] = 0.5f; m_pDiffuse[3] = 1.0f;
	m_pSpecular[0] = 0.5f; m_pSpecular[1] = 0.5f; m_pSpecular[2] = 0.5f; m_pSpecular[3] = 1.0f;

	m_bTransp	=	false;
	m_iTexId		=	0;
}

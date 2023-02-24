/*
 *  Materiau.h
 *  PhysicEngine
 *
 *  Created by Julien Peyre and Grégory Corgié on 29/11/06.
 *  Copyright 2006. All rights reserved.
 
 *
 */

#ifndef __MATERIAU_h__ 

#define __MATERIAU_h__

class Materiau 
{
	
public:
	
	Materiau();
	Materiau( int _TexId, float _fFrot, float _fGliss);
	Materiau( float fAmortCoef);
	
	// ---------------------------------------------------------------
	// Accessors
	// ---------------------------------------------------------------
	float 		GetAmortCoef()  				{ return m_fAmortCoef; }
	float		GetFrottementGlissementCoef()	{ return m_fFrottementGlissementCoef; }
	float* 		GetAmbient()					{ return m_pAmbient; }
	float* 		GetDiffuse()					{ return m_pDiffuse; }
	float* 		GetSpecular()					{ return m_pSpecular; }
	int			GetTexId()						{ return m_iTexId; }

	void		SetTexTransp( bool _b )			{ m_bTransp = _b; }

	bool		IsTransparent()					{ return m_bTransp;}
	
private:
		
	// ---------------------------------------------------------------
	// Attributs
	// ---------------------------------------------------------------
	float		m_fAmortCoef;
	float		m_fFrottementGlissementCoef;
	
	float 		m_pAmbient[4];
	float		m_pDiffuse[4];
	float 		m_pSpecular[4];

	int			m_iTexId;
	bool		m_bTransp;
};

#endif
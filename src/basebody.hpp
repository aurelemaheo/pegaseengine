/*
 *  basebody.hpp
 *  PegaseEngine
 *
 *  Created by Julien Peyre and Gr√©gory Corgi√© on 17/11/06.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef __BASEBODY_H__ 
#define __BASEBODY_H__

#include "vector3.hpp"
#include "materiau.hpp"
#include "force.hpp"

#include <list>

using namespace std;

enum BODY_TYPE
{
	BODY_SPHERE = 0,
	BODY_PLAN = 1,
	BODY_BOX = 2
};

class BaseBody 
{
	
public:
	
	BaseBody(){ m_pMateriau = NULL; m_iNbCollision=0; m_fDeltaPosition = 0.0f; }
	
	// ---------------------------------------------------------------
	// Accessors
	// ---------------------------------------------------------------
	Vector3 		GetPos() 								{ return m_vPos; }
	Vector3 		GetVelocity()							{ return m_vVelocity; }
	Vector3 		GetAcceleration()						{ return m_vAcceleration; }
	Vector3 		GetNewPos() 							{ return m_vNewPos; }
	Vector3			GetNewVelocity()						{ return m_vNewVelocity; }
	Vector3 		GetNewAcceleration()					{ return m_vNewAcceleration; }
	Vector3 		GetColVelocity()						{ return m_vCollisionSpeed; }
	Vector3			GetColPosition()						{ return m_vCollisionPosition; }
	float			GetDeltaPosition()						{ return m_fDeltaPosition; }
	int     		GetType()								{ return m_iType; }
	float     		GetMass()								{ return m_fMass; }
	Materiau* 		GetMateriau()							{ return m_pMateriau; }
	bool			IsImmobile()							{ return m_bIsImmobile; }
	

	void			SetPos( Vector3 V ) 								{ m_vPos = V; }
	void			SetVelocity( Vector3 V ) 							{ m_vVelocity = V; }
	void			SetAcceleration( Vector3 V ) 						{ m_vAcceleration = V; }
	void			SetNewPos( Vector3 V ) 								{ m_vNewPos = V; }
	void			SetNewVelocity( Vector3 V ) 						{ m_vNewVelocity = V; }
	void			SetNewAcceleration( Vector3 V ) 					{ m_vNewAcceleration = V; }
	void			SetMass( float Mass )								{ m_fMass = Mass; }
	void			SetCollisionPosition( Vector3 _vPos )				{ m_vCollisionPosition = _vPos;}
	void			SetCollision( Vector3 _vSpeed, float _vDelta)		{ m_vCollisionSpeed = _vSpeed; m_fDeltaPosition = _vDelta ; m_bIsInCollision = true; m_iNbCollision++; }
	void			SetMateriau( Materiau* _pMat )						{ m_pMateriau = _pMat; }				
	void			SetImmobile( bool _f)								{ m_bIsImmobile = _f; }

	bool			IsInCollision()							{ return m_bIsInCollision; }
	void			ResetIsInCollision()					{ Vector3 _v(0.0f,0.0f,0.0f); m_vCollisionSpeed = _v; m_fDeltaPosition= 0.0f; m_bIsInCollision = false; m_iNbCollision=0; }

	// ---------------------------------------------------------------
	// Method
	// ---------------------------------------------------------------
	void		AddVariableForce( Force _Force )			{ m_ForcesVariables.push_back(_Force); }
	void		RemoveVariableForces( )						{ m_ForcesVariables.clear(); }
	void		AddConstantForce( Vector3 _Force )			{ m_ForcesConstantes += _Force; }
	Vector3		GetConstantForces()							{ return m_ForcesConstantes; }

	virtual void Display() = 0;
	
	// ---------------------------------------------------------------
	// Public Attributs
	// ---------------------------------------------------------------
	
	//Liste de forces
	list<Force>	  m_ForcesVariables;
	Vector3		  m_ForcesConstantes;
	
	
protected:
	
	// ---------------------------------------------------------------
	// Attributs
	// ---------------------------------------------------------------
	int 		m_iType;
	float		m_fMass;
	int			m_iNbCollision;
	
	// Dynamic parameters
	Vector3		m_vInitPos;
	Vector3		m_vPos;
	Vector3		m_vVelocity;
	Vector3		m_vAcceleration;
	
	Vector3		m_vNewPos;
	Vector3		m_vNewVelocity;
	Vector3		m_vNewAcceleration;
	
	Vector3		m_vCollisionSpeed;
	Vector3		m_vCollisionPosition;
	float		m_fDeltaPosition;
	
	// Material
	Materiau* 	m_pMateriau;
	
	bool		m_bIsInCollision;
	bool		m_bIsImmobile;
};

#endif // __BASEBODY_H__

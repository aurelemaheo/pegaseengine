#ifndef __CAMERA__H__
#define __CAMERA__H__

#include "vector3.hpp"
#include "basebody.hpp"

#define MOUSE_SENSITIVITY 0.005f

class Camera
{

public:
	
	Camera( );
	
	// ---------------------------------------------------------------------
	// Attributs
	// ---------------------------------------------------------------------
	Vector3	m_vPos, m_vLookAt;

	Vector3	m_vSight;
	Vector3	m_vRight;
	Vector3	m_vUp;

	BaseBody*	m_pFollowObj;

	float		m_fDist;
	float		m_fZAngle;
	float		m_fYAngle;

	int			m_ix;
	int			m_iy;

	int			m_iPressedButton;

	bool		m_bIsHold;
	bool		m_bIsFollow;

	// ---------------------------------------------------------------------
	// Methods
	// ---------------------------------------------------------------------
	void		MoveOn( float _f);
	void		StrafeOn( float _f);
	void		RotateOn( float _fx, float _fy);
	void		FollowOn( BaseBody* _pObj ) { m_bIsFollow = true; m_pFollowObj = _pObj; }
	void		Update();

	// ---------------------------------------------------------------------
	// Accesors
	// ---------------------------------------------------------------------
	void		SetFollow( bool _b ) { m_bIsFollow = _b; }

private:
	


};

#endif

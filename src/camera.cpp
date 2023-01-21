#include "camera.hpp"
#include "vector3.hpp"

Camera::Camera()
{
	m_fDist		=		10.f;
	m_vPos		=		Vector3 ( 0.0f, 5.0f, 10.0f );
	m_vUp		=		Vector3( 0.f, 1.f, 0.f);

	// Sight
	m_vSight	=		Vector3( 0.f, 0.f, 1.f);;
	m_vRight	=		m_vUp.ProdVectoriel(m_vSight);

	m_vLookAt	=		m_vPos	+	+ m_fDist * m_vSight;
}

void Camera::MoveOn( float _f)
{
	m_vPos		+=	_f * m_vSight;

	m_vLookAt	=	m_vPos + m_fDist * m_vSight;
}

void Camera::StrafeOn( float _f )
{
	m_vPos		-=	_f * m_vRight;
	
	m_vLookAt	=	m_vPos + m_fDist * m_vSight;
}

void Camera::RotateOn( float _fx, float _fy )
{
	m_vSight		-=	_fx * MOUSE_SENSITIVITY * m_vRight;
	m_vSight		-=	_fy * MOUSE_SENSITIVITY * m_vUp;
	m_vSight		=	m_vSight.Normaliser();

	// Recompute Right Vector (should be normalized)
	m_vRight	=		m_vUp.ProdVectoriel(m_vSight);
	
	m_vLookAt	=	m_vPos + m_fDist * m_vSight;
}

void Camera::Update( )
{
	if ( m_bIsFollow )
	{
		m_vLookAt	=	m_pFollowObj->GetPos();

		m_vSight	=	m_vLookAt - m_vPos;
		m_vSight	=	m_vSight.Normaliser();
	
		// Recompute Right Vector (should be normalized)
		m_vRight	=		m_vUp.ProdVectoriel(m_vSight);
	}
}

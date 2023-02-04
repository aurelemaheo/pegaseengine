/*
 *  Scene.h
 /*
  *  PhysicEngine
  *
  *  Created by Julien Peyre and Gr√©gory Corgi√© on 17/11/06.
  *  Copyright 2006 __MyCompanyName__. All rights reserved.
  *
  */
 
#ifndef __COLLIDER_H_
#define __COLLIDER_H__
 
#include "box.hpp"
#include "sphere.hpp"
#include "plan.hpp"
#include "materiau.hpp"

using namespace std;
 
#include <list>
#include <assert.h>
 
struct Intersection
{
	Intersection( BaseBody* _A, BaseBody* _B){ A = _A; B = _B; }
	Intersection( BaseBody* _A, BaseBody* _B, Vector3 _vNormal,Vector3 _vContactPoint)
	{ A = _A; B = _B;vNormal=_vNormal;vContactPoint=_vContactPoint; }
	BaseBody* A;
	BaseBody* B;
	Vector3 vNormal;
	Vector3 vContactPoint;
};

class Collider
{
	 
public:
	 
	 Collider();
	 ~Collider(){}
	 
	 // ---------------------------------------------------------------	
	 // Methods
	 // ---------------------------------------------------------------   
	 bool 			AddObject(BaseBody* object) 	{ assert( object); m_ListBodies.push_back(object); 
												  m_iNbBodies++; return true;}

	 bool 			AddMaterial(Materiau* _mat) 	{ assert( _mat ); m_ListMat.push_back(_mat); 
												  m_iNbMat++; return true;}
	 
	 
	 void 				Display();
	 void				ApplyForces();
	 void				Update();
	 void				Reinit();
	 void				AddIntersection(BaseBody* A, BaseBody*B);
	 void				AddIntersection(BaseBody* A, BaseBody*B, Vector3 vNormal, Vector3 vContactPoint);
	 bool				IsInsidePlan(BaseBody* A, Plan* B, float _fDist);
	 int                TestSeparatingAxes(float expr1,float expr2,float n1,float n2,float n3, float* s,Vector3* normalR,Vector3* normalC,int* invert_normal);


	 // Collision functions
	 // ---------------------------------------------------------------   

	 // Méthodes de detection
	 void				TestCollision(list<BaseBody*>::iterator *it, int iNumeroObjet);
	 bool				TestSphereSphereCollision(Sphere *sphereA, Sphere *sphereB);
	 bool				TestSphereBoxCollision( Sphere* _pSphere, Box* _pBox);
	 bool				TestPlanSphereCollision(Sphere *sphere, Plan *plan);
	 bool				TestBoxPlanCollision( Box* _pBox, Plan*	_pPlan );
 	 bool				TestBoxBoxCollision(Box* _box1, Box* _box2, Vector3 *vNormal, Vector3 *vContactPoint);


	 // Méthodes de traitement
	 void				HandleSphereSphereCollision(Sphere* sphereA, Sphere* sphereB);
	 void				HandleSpherePlanCollision(Sphere* sphere, Plan* plan);
	 void				HandleSphereBoxCollision( Sphere* _pSphere, Box* _pBox);
	 void				HandleBoxPlanCollision(Box* _pBox, Plan* plan);
	 void				HandleBoxBoxCollision( Box* _pBox1, Box* _pBox2, Vector3 vNormal, Vector3 vContactPoint);
	 // Autres 
	 float				FindDeltaT(Box* _pBox, Plan* plan, Vector3 ContactPoint, Vector3 BoxVelocity);
	 Vector3			FindNormal( Sphere* _pSphere, Box* _pBox );
	 Vector3			FindImpulse( BaseBody* _pA, BaseBody* _pB, Vector3	_ColPt ){ return Vector3(0.f,0.f,0.f);}

	 // ---------------------------------------------------------------	
	 // Accessor
	 // ---------------------------------------------------------------   
	 void	SetTimeElapsed( float time )			{ m_dTimeElapsed = time; }
	 void	SetCurrentTime( float CurrentTime )	{ m_dCurrentTime = CurrentTime; }
	 void	Setdt( float dt )						{ m_dDt = dt; }
	 
	 float 				GetTimeElapsed()			{ return m_dTimeElapsed; }
	 float 				GetCurrentTime()			{ return m_dCurrentTime; }
	 float 				Getdt()						{ return m_dDt; }
	 
	 list<BaseBody*>* 	GetListBodies()		{ return &m_ListBodies; }
	 Materiau*			GetMaterial( int _id );
	 
private:
		 
	 // ---------------------------------------------------------------	
	 // Attributs
	 // ---------------------------------------------------------------
	 list<BaseBody*> 		m_ListBodies;
 	 list<Materiau*> 		m_ListMat;

	 int 			m_iNbBodies;
	 int			m_iNbMat;

	 float			m_dTimeElapsed;  	//Temps ÈcoulÈ depuis le debut de la simulation
	 float			m_dCurrentTime;		//Temps pour la frame considÈrÈe
	 float			m_dDt;				//Temps ecoulÈ entre 2 frames

	 list<Intersection> m_ListInter;

	 
 }; // class Collider
 
#endif // __COLLIDER_H__ 
 
 

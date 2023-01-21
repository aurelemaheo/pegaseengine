/*
 *  Scene.cpp
 *  PhysicEngine
 *
 *  Created by GrâˆšÂ©gory CorgiâˆšÂ© on 17/11/06.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */

#include "config.hpp"

#ifdef __MAC_OS__
#include <GLUT/glut.h>
#endif

#ifdef __WINDOWS__
#include <stdlib.h>
#include <GL/glut.h>
#endif

#include <list>
#include <iostream>
#include <math.h>

#include "scene.hpp"
#include "sphere.hpp"
#include "force.hpp"
#include "matrix.hpp"


// -------------------------------------------------------------------------------------------------------	
// Name : Constructor
// Goal : Constructor
// Params : None
// -------------------------------------------------------------------------------------------------------	
Scene::Scene()
{
	m_iNbBodies	=	0;
	m_dTimeElapsed	=	0;
	m_dCurrentTime	=	0;
	m_dDt			=	0;	
}

// -------------------------------------------------------------------------------------------------------	
// Name : Display
// Goal : Display all objects in the scene at their current position
// Params : None
// -------------------------------------------------------------------------------------------------------	
void Scene::Display()
{
	list<BaseBody*>::iterator itList = this->GetListBodies()->begin();
	
	// Call specifics display methods
	while( itList!=this->GetListBodies()->end() )
	{	
		(*itList)->Display();
		++itList;
	}
}


// -------------------------------------------------------------------------------------------------------	
// Name : ApplyForces
// Goal : Apply forces to every object
// Params : None
// -------------------------------------------------------------------------------------------------------	
void Scene::ApplyForces()
{
	Vector3 Acceleration, AccelerationAngulaire;
	Vector3 Velocity, VitesseAngulaire;
	Vector3 Position;
	float vAngle;
	Vector3 SommeDesForces, SommeDesMoments;
	
	list<BaseBody*>::iterator it_object = m_ListBodies.begin();
	
	// For each object
	while( it_object != m_ListBodies.end() )
	{
		if( (*it_object)->GetType() != BODY_PLAN && !(*it_object)->IsImmobile())
		{
			// Init
			SommeDesForces = Vector3( 0.0f, 0.0f, 0.0f);
			SommeDesMoments = Vector3( 0.0f, 0.0f, 0.0f );

			// If the object is colliding smthg 
			// -------------------------------------------------------------------------------------------------------------
			if ( (*it_object)->IsInCollision() == true )
			{	
				if( (*it_object)->GetVelocity().y>0 && (*it_object)->GetVelocity().Module()<0.05
					&& (*it_object)->GetColVelocity().Module() > (*it_object)->GetVelocity().Module() )
					int r1=1;
				
				(*it_object)->SetAcceleration( Vector3( 0.0f, 0.0f, 0.0f) );
				(*it_object)->SetVelocity( (*it_object)->GetColVelocity() );
				(*it_object)->SetPos( (*it_object)->GetColPosition() );

				if((*it_object)->GetType() == BODY_BOX)
				{
					Box* _pBox = (Box*)(*it_object);
					_pBox->UpdateRotation();
				}

				(*it_object)->ResetIsInCollision();
				
			}
			
			
			// Mecanique classique
			// -------------------------------------------------------------------------------------------------------------
			
			list<Force>::iterator it_forces = (*it_object)->m_ForcesVariables.begin();
			
			// Sum all forces that are interacting with the object
			while( it_forces != (*it_object)->m_ForcesVariables.end() )
			{
				SommeDesForces += (*it_forces).GetForce();
				SommeDesMoments += ((*it_forces).GetForce()).ProdVectoriel((*it_forces).GetApplicationPoint());
				++it_forces;
			}
			
			SommeDesForces += (*it_object)->GetConstantForces();

			// Dynamic laws
			Acceleration = SommeDesForces / (*it_object)->GetMass();
			(*it_object)->SetNewAcceleration( Acceleration );
			
			Velocity = (*it_object)->GetVelocity() + Acceleration * m_dDt;
			(*it_object)->SetNewVelocity( Velocity );

			Position = (*it_object)->GetPos() + Acceleration * m_dDt * m_dDt * 1/2 + m_dDt * (*it_object)->GetVelocity();
			(*it_object)->SetNewPos( Position );
		}


		// -------------------------------------------------------------------------------------------------------------------------
		// Greg 11/01 : Patch pour updater la rotation des boites, d'ailleurs, ceci devrait 
		//					s'étendre aux spheres
		if( (*it_object)->GetType() == BODY_BOX )
		{	
			Box* CurrentBox = (Box *) (*it_object);
			AccelerationAngulaire = CurrentBox->GetInvInertieMatrix() * SommeDesMoments;
			VitesseAngulaire = CurrentBox->GetVitesseAngulaire() + AccelerationAngulaire * m_dDt;
			vAngle = (AccelerationAngulaire.Module() * m_dDt * m_dDt * 1/2) + (m_dDt * CurrentBox->GetVitesseAngulaire().Module());
			CurrentBox->SetVitesseAngulaire( VitesseAngulaire );
			CurrentBox->SetAngle( vAngle );

			CurrentBox->UpdateRotation( );

		}
		// -------------------------------------------------------------------------------------------------------------------------


		(*it_object)->RemoveVariableForces();
		++it_object;
	}
	// Fin de la boucle de calculs de la dynamique
}

// -------------------------------------------------------------------------------------------------------	
// Name : Update
// Goal : Update the scene (move the objects), detect collisions
// Params : None
// -------------------------------------------------------------------------------------------------------	
void Scene::Update()
{
	int iNumeroObjet = 0;
	list<BaseBody*>::iterator it_object = m_ListBodies.begin();
	
	while( it_object != m_ListBodies.end() ) //Test collisions
	{
		if( (*it_object)->GetType() != BODY_PLAN )
		{
			TestCollision(&it_object,iNumeroObjet);
		}	
		++it_object;
		iNumeroObjet++;
		
	}

	//Gere les collisions (calcul des vitesse réfléchie, forces de reaction...)
	list<Intersection>::iterator it = m_ListInter.begin();
	while( it != m_ListInter.end() )
	{
		BaseBody* A,*B;
		A = (*it).A; B = (*it).B;
		
		int typeA = A->GetType();
		int typeB = B->GetType();

		if(	typeA == BODY_SPHERE	&&		typeB == BODY_SPHERE )
		{
			GererSphereSphereCollision( (Sphere*) A, (Sphere*) B);
		}
		if( typeA == BODY_SPHERE	&&		typeB == BODY_PLAN)
		{
			GererSpherePlanCollision( (Sphere*) A, (Plan*) B);
		}
		if( typeA == BODY_SPHERE	&&		typeB == BODY_BOX)
		{
			GererSphereBoxCollision( (Sphere*) A, (Box*) B);
		}
		if( typeA == BODY_BOX	&&		typeB == BODY_PLAN )
		{
			GererBoxPlanCollision( (Box*) A, (Plan*) B);
		}
		if( typeA == BODY_BOX	&&		typeB == BODY_BOX )
		{
			GererBoxBoxCollision( (Box*) A, (Box*) B, (*it).vNormal, (*it).vContactPoint);
		}
		++it;
	}
	m_ListInter.clear();

	it_object = m_ListBodies.begin();
	while( it_object != m_ListBodies.end() ) //Update object's attributes
	{
		if( (*it_object)->GetType() != BODY_PLAN )
		{
			(*it_object)->SetAcceleration( (*it_object)->GetNewAcceleration() );
			(*it_object)->SetVelocity( (*it_object)->GetNewVelocity() );
			(*it_object)->SetPos(  (*it_object)->GetNewPos() );

			//Update Forces
			if( (*it_object)->GetType() == BODY_SPHERE )
			{	
				//Force de frottement de l'air
				Vector3 Frottement = (*it_object)->GetVelocity();
				Sphere *sphere = (Sphere*)(*it_object);
				//Loi de Stokes
				Frottement = -6 * PI * 0.0000181 * sphere->GetRayon() * Frottement;
				Vector3 Application(0.0f,0.0f,0.0f);
				Force frot(Frottement,Application);
				sphere->AddVariableForce(frot);
			}
		}	
		++it_object;
	}
	
}


// -------------------------------------------------------------------------------------------------------------------------	
// Name : GetMaterial
// Goal : Retourne un pointeur à partir d'une id de materiau
// Params : _id	: id du Materiau souhaité
// -------------------------------------------------------------------------------------------------------------------------
Materiau* Scene::GetMaterial( int _id )
{
	list<Materiau*>::iterator		it;

	// On s'assure que l'id est valide
	if ( _id > m_ListMat.size() )
		return (Materiau*) *m_ListMat.begin ();	// On retourne le materiau par default ( exist tjrs )


	it = m_ListMat.begin ();
	
	for ( int i = 0; i < _id; i++)
		++it;
		
	return (Materiau*) *it;
}


// -------------------------------------------------------------------------------------------------------------------------	
// Name : IsInsidePlan
// Goal : Test si A est à l'interieur du _pPlan B
// Params : A		: objet testé 
//			_pPlan	: _pPlan testé
//			_fDist	: distance objet / _pPlan
// -------------------------------------------------------------------------------------------------------------------------
bool Scene::IsInsidePlan( BaseBody* A, Plan* _pPlan, float _fDist )
{
	Vector3		V2, ProjectionPlan;
	Vector3		CotePlan1,CotePlan2,CotePlan3,CotePlan4;
	int			Sens, SensCourant;	

	// On place le centre de l'objet A sur la _pPlan 
	V2				= _pPlan->GetNormal();
	ProjectionPlan	= A->GetNewPos() - _fDist * V2;
	
	// Calcul des cotes du _pPlan	
	CotePlan1 = _pPlan->GetPoint2() - _pPlan->GetPoint1();
	CotePlan2 = _pPlan->GetPoint3() - _pPlan->GetPoint2();
	CotePlan3 = _pPlan->GetPoint4() - _pPlan->GetPoint3();
	CotePlan4 = _pPlan->GetPoint1() - _pPlan->GetPoint4();
		
	// On regarde si l'objet est à gauche ou a droite
	Sens = ( ProjectionPlan - _pPlan->GetPoint1() ) * CotePlan1;
	
	if( Sens != 0 )
		Sens = Sens / abs(Sens);
		
	SensCourant = ( ProjectionPlan - _pPlan->GetPoint2() ) * CotePlan2;
		
	if( SensCourant != 0 ) 
		SensCourant = SensCourant / abs(SensCourant);

	if( SensCourant == Sens )
	{
		SensCourant = ( ProjectionPlan - _pPlan->GetPoint3() ) * CotePlan3;

		if( SensCourant != 0 ) 
			SensCourant = SensCourant / abs(SensCourant);
			
		if( SensCourant == Sens )
		{
			SensCourant = (ProjectionPlan - _pPlan->GetPoint4() ) * CotePlan4;
				
			if( SensCourant != 0 ) 
				SensCourant = SensCourant / abs(SensCourant);

			if( SensCourant == Sens )
					return true;
	
		}
	}

	return false;
}


// -------------------------------------------------------------------------------------------------------------------------	
// Name : TestCollision
// Goal : Se contente d'appeler les bonnes méthodes de collision en fonction des objets
// Params : it				: objet testé 
//			iNumeroObjet	: Plan considéré
// -------------------------------------------------------------------------------------------------------------------------
void Scene::TestCollision( list<BaseBody*>::iterator *it, int iNumeroObjet )
{
	int iNumeroObjetCourant = 0;
	list<BaseBody*>::iterator it_object = m_ListBodies.begin();
	

	// Parcours tous les objets de la scene
	// --------------------------------------
	while( it_object != m_ListBodies.end() )
	{
		// On ne cherche pas de collision avec le meme objet
		if( iNumeroObjetCourant != iNumeroObjet )
		{
			int typeA = (**it)->GetType();
			int typeB = (*it_object)->GetType();

			// Collision Sphere / Sphere
			if( typeA==BODY_SPHERE && typeB==BODY_SPHERE )
			{
				if ( TestSphereSphereCollision((Sphere*)(**it), (Sphere*)(*it_object)) )
					AddIntersection( (**it),(Sphere*)(*it_object) );
			}

			// Collision Sphere / Plane
			if( typeA==BODY_SPHERE && typeB==BODY_PLAN)
			{
				if ( TestPlanSphereCollision((Sphere*)(**it),(Plan*)(*it_object)) )
					AddIntersection( (**it),(Plan*)(*it_object) );
			}

			// Collision Sphere / Box
			if( typeA==BODY_SPHERE && typeB==BODY_BOX )
			{
				if ( TestSphereBoxCollision((Sphere*)(**it), (Box*)(*it_object)) )
					AddIntersection( (**it),(Sphere*)(*it_object) );
			}

			// Collision Box / Plan
			if ( typeA==BODY_BOX && typeB==BODY_PLAN)
			{
				if ( TestBoxPlanCollision( (Box*) (**it),(Plan*) (*it_object)) )
					AddIntersection( (**it),(Plan*)(*it_object) );
			}

			// Collision Box / Box
			if( typeA==BODY_BOX && typeB==BODY_BOX)
			{
				Vector3 vNormal,vContactPoint;
				if( TestBoxBoxCollision( (Box*) (**it),(Box*) (*it_object),&vNormal,&vContactPoint) )
				{AddIntersection( (**it),(Box*)(*it_object),vNormal,vContactPoint );}
			}	
			
		}
		++it_object;
		iNumeroObjetCourant++;
	}
	// --------------------------------------

}

bool Scene::TestSphereSphereCollision(Sphere *sphereA, Sphere *sphereB)
{
	Vector3 V = sphereB->GetNewPos() - sphereA->GetNewPos();
	float norme = V.Module();

	if( norme <= sphereA->GetRayon() + sphereB->GetRayon() )
	{

		return true;
	}
	return false;
	
}

bool Scene::TestPlanSphereCollision(Sphere *sphere, Plan *_pPlan)
{
	Vector3 V1 = sphere->GetNewPos() - _pPlan->GetPos();
	Vector3 V2 = _pPlan->GetNormal();
	float d = V1*V2;
	
	// Intersection dâˆšÂ©tectâˆšÂ©e
	if( fabs(d) <= sphere->GetRayon() )
	{
		return IsInsidePlan( sphere, _pPlan, d);
	}
	
	return false;
}

// -------------------------------------------------------------------------------------------------------	
// Name : Reinit
// Goal : Reinit all objects to their initial positions
// Params : None
// --------------------------------------------------------------	
void Scene::Reinit()
{
	this->m_ListBodies.clear();
	this->m_ListInter.clear();
}


void Scene::AddIntersection(BaseBody* A, BaseBody*B)
{
	list<Intersection>::iterator it = m_ListInter.begin();
	bool Already = false;
	//Evite les doublons dans la liste
	while( it != m_ListInter.end() )
	{
		if( (*it).A == B && (*it).B == A )
		{
			Already=true;
			break;
		}
		++it;
	}
	if( Already==false )
		m_ListInter.push_back( Intersection(A,B) );

}

void Scene::AddIntersection(BaseBody* A, BaseBody*B, Vector3 vNormal, Vector3 vContactPoint)
{
	list<Intersection>::iterator it = m_ListInter.begin();
	bool Already = false;
	//Evite les doublons dans la liste
	while( it != m_ListInter.end() )
	{
		if( (*it).A == B && (*it).B == A )
		{
			Already=true;
			break;
		}
		++it;
	}
	if( Already==false )
		m_ListInter.push_back( Intersection(A,B,vNormal,vContactPoint) );

}

void Scene::GererSphereSphereCollision(Sphere* sphereA, Sphere* sphereB)
{
	Vector3 V = sphereB->GetNewPos() - sphereA->GetNewPos();
	float norme = V.Module();
	V.Normaliser();

	//Find Velocities
	Vector3 VelocityA,VelocityB;
	if(sphereA->IsInCollision() == true)
		VelocityA=sphereA->GetColVelocity();
	else
		VelocityA=sphereA->GetNewVelocity();

	if(sphereB->IsInCollision() == true)
		VelocityB=sphereB->GetColVelocity();
	else
		VelocityB=sphereB->GetNewVelocity();
	
	float 	AmortCoef 	= sphereA->GetMateriau()->GetAmortCoef() * sphereA->GetMateriau()->GetAmortCoef();
	float   fM1 = sphereA->GetMass();
	float   fM2 = sphereB->GetMass();
	
	Vector3 U1x = (V * VelocityA) * V; 
	Vector3 U1y = VelocityA - U1x;
	Vector3 U2x = (-V * VelocityB) * (-V); 
	Vector3 U2y = VelocityB - U2x;
	
	//Find new velocity
	Vector3 V1x = ( (U1x*fM1) + (U2x*fM2) - (U1x-U2x)*fM2 )/(fM1+fM2);
	Vector3 V1y = U1y;
	Vector3 V2x = ( (U1x*fM1) + (U2x*fM2) - (U2x-U1x)*fM1 )/(fM1+fM2);
	Vector3 V2y = U2y;
	
	Vector3 vReflechie1 = V1x+V1y;
	Vector3 vReflechie2 = V2x+V2y;
	
	//Amortissement?????
	Vector3 VelocityNormA = VelocityA;
	VelocityNormA.Normaliser();
	Vector3 VelocityNormB = VelocityB;
	VelocityNormB.Normaliser();

	float PenteVitesse1 = fabs( (VelocityA-(VelocityA*V)*V).Module()/(VelocityA*V));
	float PenteVitesse2 = fabs( (VelocityB-(VelocityB*V)*V).Module()/(VelocityB*V));

	Vector3 vNewPos;
	float DeltaPos1;
	float DeltaPos2;
	
	// TODO : Greg 21/01, il y avait une division par zero sinon
	if ( PenteVitesse1 == 0.f	&& PenteVitesse2 == 0.f ) 
	{
		DeltaPos1	=	0.001f;
		DeltaPos2	=	0.001f;
	}
	else
	{
		DeltaPos1= PenteVitesse2*(sphereA->GetRayon()+sphereB->GetRayon()-norme)/(PenteVitesse1+PenteVitesse2);
		DeltaPos1 = DeltaPos1 * sqrt(1+PenteVitesse1*PenteVitesse1);
		DeltaPos2 = PenteVitesse1*(sphereA->GetRayon()+sphereB->GetRayon()-norme)/(PenteVitesse1+PenteVitesse2);
		DeltaPos2 = DeltaPos2 * sqrt(1+PenteVitesse2*PenteVitesse2);
	}

	float CurrentDeltaPos = sphereA->GetDeltaPosition();

	if( DeltaPos1 > CurrentDeltaPos )
	{
		vNewPos = sphereA->GetNewPos()-DeltaPos1*VelocityNormA;
		sphereA->SetCollisionPosition( vNewPos );
		sphereA->SetCollision(vReflechie1, DeltaPos1 );
	}
	else
		sphereA->SetCollision(vReflechie1, CurrentDeltaPos );

	CurrentDeltaPos = sphereB->GetDeltaPosition();
	
	if( DeltaPos2 > CurrentDeltaPos )
	{
		vNewPos = sphereB->GetNewPos()-DeltaPos2*VelocityNormB;
		sphereB->SetCollisionPosition( vNewPos );
		sphereB->SetCollision(vReflechie2, DeltaPos1 );
	}
	else
		sphereB->SetCollision(vReflechie2, CurrentDeltaPos );

	//Force de reaction
	if(V.y!=0)
	{
		Vector3 ForceVerticale( 0.0f, sphereA->GetConstantForces().y, 0.0f );
		Vector3 ForceReaction =  fabs(ForceVerticale.y/V.y)*V;
		Vector3 Application(0.0f,0.0f,0.0f);
		Force reaction(ForceReaction,Application);
		sphereA->AddVariableForce( reaction );

		ForceVerticale.Set( 0.0f, sphereB->GetConstantForces().y, 0.0f );
		ForceReaction =  fabs(ForceVerticale.y/V.y)*V;
		reaction.SetForce(ForceReaction);
		sphereB->AddVariableForce( reaction );
	}

}

// -------------------------------------------------------------------------------------------------------	
// Name : TestSphereBoxCollision
// Goal : Test si une boite touche un _pPlan, enregistre l'intersection si vrai
// Params : _pBox			: Boite considérée
//				_pSphere	: Sphere considérée
// -------------------------------------------------------------------------------------------------------
bool Scene::TestSphereBoxCollision( Sphere* _pSphere, Box* _pBox)
{
	Vector3		vPNormal;
	float			fR1Proj, fR2Proj, fR3Proj;
	float			fR1Dot, fR2Dot, fR3Dot;
	float			fDepth;
	float			fSphrBoxD;

	if ( !_pBox || !_pSphere )
		return false;

	vPNormal	=	_pSphere->GetNewPos() -  _pBox->GetNewPos();
	fSphrBoxD	=	vPNormal.Module();
	vPNormal.Normaliser();

	// Projection du repère local du cube selon la droite passant par le centre des 2 obj
	fR1Dot	=	vPNormal * _pBox->GetR1();
	fR2Dot	=	vPNormal * _pBox->GetR2();
	fR3Dot	=	vPNormal * _pBox->GetR3();

	fR1Proj	=	fabs( _pBox->GetDim1() * fR1Dot );
	fR2Proj	=	fabs( _pBox->GetDim2() * fR2Dot );
	fR3Proj	=	fabs( _pBox->GetDim3() * fR3Dot );
	

	// Calcul de l'interpenetration entre les deux objets
	fDepth	=	fSphrBoxD - 0.5f * ( fR1Proj + fR2Proj +fR3Proj ) - _pSphere->GetRayon();

	if (  fDepth <= 0.0f )
		return true;

	return false;
}

void Scene::GererSpherePlanCollision(Sphere* sphere, Plan* _pPlan)
{
	Vector3 V1 = sphere->GetNewPos() - _pPlan->GetPos();
	Vector3 V2 = _pPlan->GetNormal();
	float d = V1*V2;

	Vector3 Normale = _pPlan->GetNormal();
	Vector3 SphereVelocity;
	SphereVelocity = sphere->GetNewVelocity();

	Vector3 SphereVelocityNorm = SphereVelocity;
	SphereVelocityNorm.Normaliser();
	if( Normale * SphereVelocity > 0 )
		Normale = - Normale;
	Vector3 PointIntersect;
	if(SphereVelocity*Normale!=0)
		PointIntersect = sphere->GetNewPos() - (SphereVelocity.Module()*(sphere->GetRayon()-fabs(d))/fabs(SphereVelocity*Normale))*SphereVelocityNorm - sphere->GetRayon() * Normale;
	else
		PointIntersect = sphere->GetNewPos() - sphere->GetRayon() * Normale;

	// On se base sur la vitesse des precendentes collisions pour le calcul des vitesses reflechies
	if(sphere->IsInCollision() == true )
		SphereVelocity = sphere->GetColVelocity();
	else
		SphereVelocity = sphere->GetNewVelocity();

	// Calcul de la vitesse reflechie par rapport à la normale du _pPlan
	float 	AmortCoef 	= sphere->GetMateriau()->GetAmortCoef() * _pPlan->GetMateriau()->GetAmortCoef();
	Vector3 vReflechie 	= (fabs( SphereVelocity * Normale ) * Normale) *2 + SphereVelocity;
	
	//Amortissement suivant la normale du _pPlan
	float fVitesseNormale = fabs(vReflechie*Normale);
	vReflechie = vReflechie +(fVitesseNormale - fVitesseNormale*AmortCoef)* (-Normale);
	
	float DeltaPos = (PointIntersect + Normale*sphere->GetRayon() - sphere->GetNewPos()).Module();
	float CurrentDeltaPos = sphere->GetDeltaPosition();
	
	if( DeltaPos > CurrentDeltaPos )
	{
		sphere->SetCollisionPosition( PointIntersect + Normale*sphere->GetRayon() );
		sphere->SetCollision(vReflechie , DeltaPos );
	}
	else
		sphere->SetCollision(vReflechie , CurrentDeltaPos );

	Vector3 v = PointIntersect + Normale*sphere->GetRayon();
	
	// Si le _pPlan n'est pas vertical
	if(Normale.y!=0)
	{	
		float CoeffFrot = sphere->GetMateriau()->GetFrottementGlissementCoef();
		CoeffFrot *= _pPlan->GetMateriau()->GetFrottementGlissementCoef();
		Vector3 ForceVerticale( 0.0f, sphere->GetConstantForces().y, 0.0f );
		Vector3 ForceReactionNormale =  fabs(ForceVerticale.y/Normale.y)*Normale;
		Vector3 ForceReactionTangeantielle=-(ForceVerticale-ForceReactionNormale);
		if(ForceReactionTangeantielle.Module()>fabs(ForceReactionNormale.Module()*CoeffFrot))
		{
			ForceReactionTangeantielle.Normaliser();
			ForceReactionTangeantielle = fabs(ForceReactionNormale.Module()*CoeffFrot)*ForceReactionTangeantielle;
		}
		Vector3 ForceReaction = ForceReactionNormale + ForceReactionTangeantielle;

		Vector3 Application(0.0f,0.0f,0.0f);
		Force reaction(ForceReaction,Application);
		sphere->AddVariableForce( reaction );
	}	
}

// -------------------------------------------------------------------------------------------------------	
// Name : TestBoxPlanCollision
// Goal : Test si une boite touche un plan, enregistre l'intersection si vrai
// Params : _pBox	: Boite considérée
//				_pPlan	: Plan considéré
// -------------------------------------------------------------------------------------------------------
bool Scene::TestBoxPlanCollision( Box* _pBox, Plan* _pPlan)
{
	Vector3		vPNormal, vV1;
	float		fR1Proj, fR2Proj, fR3Proj;
	float		fR1Dot, fR2Dot, fR3Dot;
	float		fDepth;
	float		fBoxPlanD;

	if ( !_pBox || !_pPlan )
		return false;

	vPNormal	=	_pPlan->GetNormal();
	vV1			=	_pBox->GetNewPos() - _pPlan->GetPos();
	fBoxPlanD	=	fabs( vV1 * vPNormal);

	if ( !IsInsidePlan( _pBox, _pPlan, fBoxPlanD) )
		return false;

	// Projection du repère local du cube selon la normale au _pPlan
	fR1Dot	=	vPNormal * _pBox->GetR1();
	fR2Dot	=	vPNormal * _pBox->GetR2();
	fR3Dot	=	vPNormal * _pBox->GetR3();

	fR1Proj	=	fabs( _pBox->GetDim1() * fR1Dot );
	fR2Proj	=	fabs( _pBox->GetDim2() * fR2Dot );
	fR3Proj	=	fabs( _pBox->GetDim3() * fR3Dot );
	

	// Calcul de l'interpenetration entre les deux objets
	fDepth	=	fBoxPlanD - 0.5f * ( fR1Proj + fR2Proj +fR3Proj );

	if (  fDepth <= 0.0f )
		return true;

	return false;
}

// ---------------------------------------------------------------	
// Name : TestBoxBoxCollision
// Goal : Test si une boite touche une autre boite, enregistre l'intersection si vrai
// Params : _pBox	: Boite1 considérée
//			_pPlan	: Boite2 considérée
// ---------------------------------------------------------------
bool Scene::TestBoxBoxCollision(Box* _box1, Box* _box2, Vector3 *vNormal, Vector3 *vContactPoint)
{
	int code; //code de retour
	float s,s2;
	int invert_normal;

	Vector3 vR11,vR12,vR13; //repere boite 1
	Vector3 vR21,vR22,vR23; //repere boite 2
	Vector3 vP = _box2->GetNewPos() - _box1->GetNewPos();
	Vector3 vP1,vP2; //vP dans les coordonnées de boite 1 et 2
	Vector3 normalR,normalC,normal;

	vR11 = _box1->GetR1(); vR12 = _box1->GetR2(); vR13 = _box1->GetR3();
	vR21 = _box2->GetR1(); vR22 = _box2->GetR2(); vR23 = _box2->GetR3();

	vP1.Set((vP*vR11),(vP*vR12),(vP*vR13));
	vP2.Set((vP*vR21),(vP*vR22),(vP*vR23));

	float fDim11,fDim12,fDim13; //dimension de la box1
	float fDim21,fDim22,fDim23; //dimension de la box2

	fDim11 = _box1->GetDim1()/2; fDim12 = _box1->GetDim2()/2; fDim13 = _box1->GetDim3()/2;
	fDim21 = _box2->GetDim1()/2; fDim22 = _box2->GetDim2()/2; fDim23 = _box2->GetDim3()/2;

	float P11,P12,P13,P21,P22,P23,P31,P32,P33,Q11,Q12,Q13,Q21,Q22,Q23,Q31,Q32,Q33;

	// Pij is R1'*R2, i.e. the relative rotation between R1 and R2
	P11 = vR11*vR21; P12 = vR11*vR22; P13 = vR11*vR23;
	P21 = vR12*vR21; P22 = vR12*vR22; P23 = vR12*vR23;
    P31 = vR13*vR21; P32 = vR13*vR22; P33 = vR13*vR23;

    Q11 = fabs(P11); Q12 = fabs(P12); Q13 = fabs(P13);
    Q21 = fabs(P21); Q22 = fabs(P22); Q23 = fabs(P23);
    Q31 = fabs(P31); Q32 = fabs(P32); Q33 = fabs(P33);

	s = -100000000; //INFINITY
    invert_normal = 0;
    code = 0;

  //Testing 15 separating axes
  // Collision sommet/face
  s2 = fabs(vP1.x) - (fDim11+fDim21*Q11+fDim22*Q12+fDim23*Q13);
  if( s2 > 0 )
	  return false;
  if( s2 > s )
  {s=s2; normalR=vR11; invert_normal=(vP1.x<0); code = 1;}
//-----------------   
  s2 = fabs(vP1.y) - (fDim12+fDim21*Q21+fDim22*Q22+fDim23*Q23);
  if( s2 > 0 )
	  return false;
  if( s2 > s )
  {s=s2; normalR=vR12; invert_normal=(vP1.y<0); code = 2;}
 //---------------- 
  s2 = fabs(vP1.z) - (fDim13+fDim21*Q31+fDim22*Q32+fDim23*Q33);
  if( s2 > 0 )
	  return false;
  if( s2 > s )
  {s=s2; normalR=vR13; invert_normal=(vP1.z<0); code = 3;}

  // separating axis = v1,v2,v3
  s2 = fabs(vP2.x) - (fDim11*Q11+fDim12*Q21+fDim13*Q31+fDim21);
  if( s2 > 0 )
	  return false;
  if( s2 > s )
  {s=s2; normalR=vR21; invert_normal=(vP2.x<0); code = 4;}
  //---------------
  s2 = fabs(vP2.y) - (fDim11*Q12+fDim12*Q22+fDim13*Q32+fDim22);
  if( s2 > 0 )
	  return false;
  if( s2 > s )
  {s=s2; normalR=vR22; invert_normal=(vP2.y<0); code = 5;}
  //---------------
  s2 = fabs(vP2.z) - (fDim11*Q13+fDim12*Q23+fDim13*Q33+fDim23);
  if( s2 > 0 )
	  return false;
  if( s2 > s )
  {s=s2; normalR=vR23; invert_normal=(vP2.z<0); code = 6;}


  //Collision arrete/arrete
 

  s2 = TestSeparatingAxes((vP1.z*P21-vP1.y*P31),(fDim12*Q31+fDim13*Q21+fDim22*Q13+fDim23*Q12),
						  0,-P31,P21,&s,&normalR,&normalC,&invert_normal);
  if(s2 == -1)
	  return false;
  if(s2 == 1)
	  code = 7;
  //---------------
  s2 = TestSeparatingAxes((vP1.z*P22-vP1.y*P32),(fDim12*Q32+fDim13*Q22+fDim21*Q13+fDim23*Q11),
						  0,-P32,P22,&s,&normalR,&normalC,&invert_normal);
  if(s2 == -1)
	  return false;
  if(s2 == 1)
	  code = 8;
  //---------------
  s2 = TestSeparatingAxes((vP1.z*P23-vP1.y*P33),(fDim12*Q33+fDim13*Q23+fDim21*Q12+fDim22*Q11),
						  0,-P33,P23,&s,&normalR,&normalC,&invert_normal);
  if(s2 == -1)
	  return false;
  if(s2 == 1)
	  code = 9;
  //---------------

   s2 = TestSeparatingAxes((vP1.x*P31-vP1.z*P11),(fDim11*Q31+fDim13*Q11+fDim22*Q23+fDim23*Q22),
						  P31,0,-P11,&s,&normalR,&normalC,&invert_normal);
  if(s2 == -1)
	  return false;
  if(s2 == 1)
	  code = 10;
  //---------------
   s2 = TestSeparatingAxes((vP1.x*P32-vP1.z*P12),(fDim11*Q32+fDim13*Q12+fDim21*Q23+fDim23*Q21),
						  P32,0,-P12,&s,&normalR,&normalC,&invert_normal);
  if(s2 == -1)
	  return false;
  if(s2 == 1)
	  code = 11;
  //---------------
   s2 = TestSeparatingAxes((vP1.x*P33-vP1.z*P13),(fDim11*Q33+fDim13*Q13+fDim21*Q22+fDim22*Q21),
						  P33,0,-P13,&s,&normalR,&normalC,&invert_normal);
   if(s2 == -1)
	  return false;
  if(s2 == 1)
	  code = 12;
  //---------------
   s2 = TestSeparatingAxes((vP1.y*P11-vP1.x*P21),(fDim11*Q21+fDim12*Q11+fDim22*Q33+fDim23*Q32),
						  -P21,P11,0,&s,&normalR,&normalC,&invert_normal);
  if(s2 == -1)
	  return false;
  if(s2 == 1)
	  code = 13;
  //---------------
   s2 = TestSeparatingAxes((vP1.y*P12-vP1.x*P22),(fDim11*Q22+fDim12*Q12+fDim21*Q33+fDim23*Q31),
						  -P22,P12,0,&s,&normalR,&normalC,&invert_normal);
   if(s2 == -1)
	  return false;
  if(s2 == 1)
	  code = 14;
  //---------------
  s2 = TestSeparatingAxes((vP1.y*P13-vP1.x*P23),(fDim11*Q23+fDim12*Q13+fDim21*Q32+fDim22*Q31),
						  -P23,P13,0,&s,&normalR,&normalC,&invert_normal);
   if(s2 == -1)
	  return false;
  if(s2 == 1)
	  code = 15;
  //---------------
 
  if(normalR.Module()==0)
  {
	  vNormal->x=vR11.x*normalC.x+vR12.x*normalC.y+vR13.x*normalC.z; //conversion to from local to global
	  vNormal->y=vR11.y*normalC.x+vR12.y*normalC.y+vR13.y*normalC.z;
  	  vNormal->z=vR11.z*normalC.x+vR12.z*normalC.y+vR13.z*normalC.z;
	  *vNormal=-*vNormal;
  }
  else
	  *vNormal=-normalR;
  if(invert_normal)
	  *vNormal=-*vNormal;
  vNormal->Normaliser();

  //Find Contact Point
  //------------------
  if( code < 4 ) 
  {
	if(vR21*(*vNormal) != 0.f)
		*vContactPoint = fDim21*vR21*(vR21*(*vNormal))/fabs(vR21*(*vNormal));
	if(vR22*(*vNormal) != 0.0f)
		*vContactPoint = *vContactPoint	+ fDim22*vR22*(vR22*(*vNormal))/fabs(vR22*(*vNormal));
	if(vR23*(*vNormal) != 0.0f)
		*vContactPoint = *vContactPoint + fDim23*vR23*(vR23*(*vNormal))/fabs(vR23*(*vNormal));
	
	*vContactPoint = *vContactPoint + _box2->GetNewPos(); //conversion coordonnée globale

	return true;
  }
  if( code > 3 && code <7 ) 
  {
	if(vR11*(*vNormal) != 0.f)
		*vContactPoint = -fDim11*vR11*(vR11*(*vNormal))/fabs(vR11*(*vNormal));
	if(vR12*(*vNormal) != 0.0f)
		*vContactPoint = *vContactPoint	- fDim12*vR12*(vR12*(*vNormal))/fabs(vR12*(*vNormal));
	if(vR13*(*vNormal) != 0.0f)
		*vContactPoint = *vContactPoint - fDim13*vR13*(vR13*(*vNormal))/fabs(vR13*(*vNormal));
	
	*vContactPoint = *vContactPoint + _box1->GetNewPos(); //conversion coordonnée globale

	return true;
  }

  if( code > 6 )
  {
	if(fabs(vR11*(*vNormal)) > 0.0005f)
		*vContactPoint = -fDim11*vR11*(vR11*(*vNormal))/fabs(vR11*(*vNormal));
	else
		*vContactPoint = vP1.x*vR11;
	if(fabs(vR12*(*vNormal)) > 0.0005f)
		*vContactPoint = *vContactPoint	- fDim12*vR12*(vR12*(*vNormal))/fabs(vR12*(*vNormal));
	else
		*vContactPoint += vP1.y*vR12;
	if(fabs(vR13*(*vNormal)) > 0.0005f)
		*vContactPoint = *vContactPoint - fDim13*vR13*(vR13*(*vNormal))/fabs(vR13*(*vNormal));
	else
		*vContactPoint += vP1.z*vR13;
	
	*vContactPoint = *vContactPoint + _box1->GetNewPos(); //conversion coordonnée globale

	return true;
  }

  return false;

}

int Scene::TestSeparatingAxes(float expr1,float expr2,float n1,float n2,float n3, float* s,Vector3* normalR,Vector3* normalC,int* invert_normal)
{
	float s2 = fabs(expr1) - (expr2);
    if (s2 > 0) return -1;
    float l = sqrtf((n1)*(n1) + (n2)*(n2) + (n3)*(n3));
    if (l > 0) 
	{ 
		s2 /= l; 
		if (s2*1.05 > *s)
		{
			*s=s2;
			normalR->Set(0.f,0.f,0.f); 
			normalC->Set(n1/l,n2/l,n3/l);
			*invert_normal = ((expr1) < 0);
			return 1;
		}
	}
	return 0;	
}

// -------------------------------------------------------------------------------------------------------	
// Name : GererBoxPlanCollision
// Goal : Gere le comportement d'une boite lors d'une collision avec un plan
// Params : _pBox			: Boite considérée
//				_pPlan	: Plan considéré
// -------------------------------------------------------------------------------------------------------
void Scene::GererBoxPlanCollision(Box* _pBox, Plan* _pPlan)
{	
	Vector3 vReflechie, vAngulaire;
	Vector3 BoxVelocity,BoxVelocityNorm;
	Vector3 ContactPoint;
	Vector3 vNormal = _pPlan->GetNormal();
	Vector3 PointIntersect;
	Vector3 vR1,vR2,vR3;
	Vector3 vV1;  //Vecteur qui lie le _pPlan et la _pBox
	Vector3 vTangeante;
	TMatrix<float> Minertie = _pBox->GetInertieMatrix();

	float fImpulsex = 0.f;
	float fImpulsey = 0.f;
	Vector3 vImpulse;
	float fDim1,fDim2,fDim3;
	float fCoeffAm = _pBox->GetMateriau()->GetAmortCoef()*_pPlan->GetMateriau()->GetAmortCoef();
	float fCoeffFr = _pPlan->GetMateriau()->GetFrottementGlissementCoef();
	float fBoxPlanD;
	float fMassBox = _pBox->GetMass();

	vV1			=	_pBox->GetNewPos() - _pPlan->GetPos();
	fBoxPlanD	=	fabs( vV1 * vNormal);

	fDim1=_pBox->GetDim1(); fDim2=_pBox->GetDim2(); fDim3=_pBox->GetDim3();
	vR1=_pBox->GetR1(); vR2=_pBox->GetR2(); vR3=_pBox->GetR3();

	if(_pBox->IsInCollision() == true )
		BoxVelocity = _pBox->GetColVelocity();
	else
		BoxVelocity = _pBox->GetNewVelocity();

	if( BoxVelocity.Module()<0.01f && BoxVelocity*vNormal<0 ) //Arret vitesse lineaire
		BoxVelocity.Set(0.f,0.f,0.f);

	if(vV1*vNormal<0)
		vNormal=-vNormal;

	//Find Contact Point
	//------------------
	
	if(vR1*vNormal != 0.f)
		ContactPoint = -fDim1/2*vR1*(vR1*vNormal)/fabs(vR1*vNormal);
	if(vR2*vNormal != 0.0f)
		ContactPoint = ContactPoint	- fDim2/2*vR2*(vR2*vNormal)/fabs(vR2*vNormal);
	if(vR3*vNormal != 0.0f)
	ContactPoint = ContactPoint - fDim3/2*vR3*(vR3*vNormal)/fabs(vR3*vNormal);

	// arret objet + recalage axe
	if ( fabs(vR1*vNormal) >= 0.999f ) 
	{
		_pBox->SetVitesseAngulaire(Vector3(0.f,0.f,0.f));
		Vector3 NewR;
		ContactPoint = -fDim1/2*vNormal;
		float signe = vR1*vNormal;
		signe /= fabs(signe);
		_pBox->SetR1(signe*vNormal);
		NewR = -(vR2*vNormal)*vNormal+vR2;
		NewR.Normaliser();
		_pBox->SetR2(NewR);
		NewR = -(vR3*vNormal)*vNormal+vR3;
		NewR.Normaliser();
		_pBox->SetR3(NewR);
	}
	if( fabs(vR2*vNormal) >= 0.999f )
	{
		_pBox->SetVitesseAngulaire(Vector3(0.f,0.f,0.f));
		Vector3 NewR;
		ContactPoint = -fDim2/2*vNormal;
		float signe = vR2*vNormal;
		signe /= fabs(signe);
		_pBox->SetR2(signe*vNormal);
		NewR = -(vR1*vNormal)*vNormal+vR1;
		NewR.Normaliser();
		_pBox->SetR1(NewR);
		NewR = -(vR3*vNormal)*vNormal+vR3;
		NewR.Normaliser();
		_pBox->SetR3(NewR);
	}
	if( fabs(vR3*vNormal) >= 0.999f )
	{
		_pBox->SetVitesseAngulaire(Vector3(0.f,0.f,0.f));
		Vector3 NewR;
		ContactPoint = -fDim3/2*vNormal;
		float signe = vR3*vNormal;
		signe /= fabs(signe);
		_pBox->SetR3(signe*vNormal);
		NewR = -(vR2*vNormal)*vNormal+vR2;
		NewR.Normaliser();
		_pBox->SetR2(NewR);
		NewR = -(vR1*vNormal)*vNormal+vR1;
		NewR.Normaliser();
		_pBox->SetR1(NewR);
	}

	//-----------------------------------------------------------------------------------------------
	//Find Intersect Point
	//-----------------------------------------------------------------------------------------------
	float DT = FindDeltaT(_pBox,_pPlan,ContactPoint,BoxVelocity);
	
	float ColAngle = 0.f;
	
	if ( DT >= 0.f )
		ColAngle = _pBox->GetVitesseAngulaire().Module() * DT;

	if ( DT == -1.f) 
		ContactPoint = -(ContactPoint*vNormal)*vNormal;

	//-----------------------------------------------------------------------------------------------
	// Vitesses Reflechies
	//-----------------------------------------------------------------------------------------------
	Vector3 VitPointCol = BoxVelocity + _pBox->GetVitesseAngulaire().ProdVectoriel(ContactPoint);
	
	Vector3 ContactN = (ContactPoint * vNormal)*vNormal; // Composante Normale de ContactPoint
	Vector3 ContactT = ContactPoint - ContactN; // Composante Tangeantielle de ContactPoint

	Vector3 AxeRotation = ContactPoint.ProdVectoriel(vNormal);
	AxeRotation.Normaliser();
	float InertiaMomentum = (Minertie * AxeRotation).Module();
	
	if(InertiaMomentum == 0.0f)
		InertiaMomentum = 1000000000.f; //INFINITY


	//-----------------------------------------------------------------------------------------------
	// Compute impulsion vector
	//-----------------------------------------------------------------------------------------------

	//Classification des mouvements
	if(ContactT.Module()!=0)
		vTangeante = -ContactT.Normalised();
	else
		vTangeante=(VitPointCol-(VitPointCol * vNormal)*vNormal).Normalised();
	
	float VitPointColN = (VitPointCol * vNormal); //Composante Normale VitPointCol
	float VitPointColT = (VitPointCol - VitPointColN*vNormal)*vTangeante; //Composante Normale VitPointCol

	float signe;
	if(VitPointColT==0)
		signe=1;
	else
		signe = VitPointColT/fabs(VitPointColT);

	float B1 = 1/fMassBox + ContactN.ModuleCarre()/InertiaMomentum;
	float B2 = 1/fMassBox + ContactT.ModuleCarre()/InertiaMomentum;
	float B3 = (ContactT.Module()*ContactN.Module())/InertiaMomentum;
	float MUs = B3/B1;
	float Pd = (B2+signe*fCoeffFr*B3)*signe*VitPointColT;
	float Pq = (fCoeffFr*B1+signe*B3)*fabs(VitPointColN);

	// Glissement
	// ----------------------------------------------------------------------------------------------------------------
	if( Pd > (1+fCoeffAm)*Pq ) 
	{
		fImpulsey = -(1+fCoeffAm)*VitPointColN/(B2+signe*fCoeffFr*B3);
		fImpulsex = -signe*fCoeffFr * fImpulsey;
	}

	// R-adherence
	// ----------------------------------------------------------------------------------------------------------------
	if( (Pq < Pd) && (Pd < (1+fCoeffAm)*Pq) && (fCoeffFr > MUs) )
	{
		fImpulsey = -(1+fCoeffAm)*VitPointColN/(B2+signe*fCoeffFr*B3);
		fImpulsex = (B3 * fImpulsey - VitPointColT)/B1;
	}

	//R-glissement
	// ----------------------------------------------------------------------------------------------------------------
	if( (Pq < Pd) && (Pd < (1+fCoeffAm)*Pq) && (fCoeffFr <= MUs) )
	{
		fImpulsey = -(1+fCoeffAm)*VitPointColN/(B2+signe*fCoeffFr*B3);
		fImpulsex = signe*fCoeffFr*(fImpulsey-2*VitPointColT/(B3+signe*fCoeffFr*B1));
	}

	//C-adherence
	// ----------------------------------------------------------------------------------------------------------------
	if( (Pd < Pq) && (fCoeffFr > MUs) )
	{
		fImpulsey = -(1+fCoeffAm)*(B1*VitPointColN+B3*VitPointColT)/(B1*B2-B3*B3);
		fImpulsex = (B3 * fImpulsey - VitPointColT)/B1;
	}

	//C-glissement
	// ----------------------------------------------------------------------------------------------------------------
	if( (Pd < Pq) && (fCoeffFr <= MUs) )
	{
		fImpulsey = VitPointColN+2*signe*fCoeffFr*B3*VitPointColT/(B3+signe*fCoeffFr*B1);
		fImpulsey *= -(1+fCoeffAm)/(B2-signe*fCoeffFr*B3);
		fImpulsex = signe*fCoeffFr*(fImpulsey-2*VitPointColT/(B3+signe*fCoeffFr*B1));
	}

	vImpulse = fImpulsey*vNormal+fImpulsex*vTangeante;

	if( BoxVelocity * vNormal < 0 )
	{
		vReflechie = BoxVelocity + vImpulse/fMassBox;
	}
	else
		vReflechie = BoxVelocity;

	vV1 =	ContactPoint.ProdVectoriel(vNormal); 
	vV1.Normaliser();

	Vector3 ContactPointPerp = -ContactT.Module()*ContactN.Normalised() + ContactN.Module()*ContactT.Normalised();
	vAngulaire = (fabs(ContactPointPerp * vImpulse)/InertiaMomentum)*vV1;
	vAngulaire	+=	_pBox->GetVitesseAngulaire(); 

	//-----------------------------------------------------------------------------------------------
	// Recalage de l'objet
	//-----------------------------------------------------------------------------------------------
	float CurrentDelta = _pBox->GetDeltaPosition();

	if( DT == -2.0 )
	{
		_pBox->SetCollisionPosition(_pBox->GetNewPos()+(fabs(ContactPoint*vNormal)-fBoxPlanD)*vNormal , 0.0f );
		_pBox->SetCollision(vReflechie , 0.0f );
	}

	else if( DT > CurrentDelta )
	{
		Vector3 VelocityNorm = BoxVelocity;
		VelocityNorm.Normaliser();
		
		_pBox->SetCollisionPosition( _pBox->GetNewPos() - (BoxVelocity * DT) , ColAngle );
		_pBox->SetCollision( vReflechie , DT );
	}
	
	// On force l'ARRET de l'objet
	if(DT == -1.0f )
	{
		_pBox->SetCollisionPosition(_pBox->GetNewPos(), 0.0f);
		_pBox->SetCollision(vReflechie , 0.0f );
	}

	_pBox->SetVitesseAngulaire(vAngulaire);

	//-----------------------------------------------------------------------------------------------
	//Force de Réaction
	//-----------------------------------------------------------------------------------------------

	// Si le _pPlan n'est pas vertical
	if(vNormal.y!=0 && ( /*BoxVelocity * vNormal <= 0 ||*/ DT==-1.f))
	{
		float CoeffFrot = _pBox->GetMateriau()->GetFrottementGlissementCoef();
		CoeffFrot *= _pPlan->GetMateriau()->GetFrottementGlissementCoef();
		Vector3 ForceVerticale( 0.0f, _pBox->GetConstantForces().y, 0.0f );
		Vector3 ForceReactionNormale =  fabs(ForceVerticale.y/vNormal.y)*vNormal;
		Vector3 ForceReactionTangeantielle=-(ForceVerticale-ForceReactionNormale);
		Force reaction;

		if(ForceReactionTangeantielle.Module()>fabs(ForceReactionNormale.Module()*CoeffFrot))
		{
			ForceReactionTangeantielle.Normaliser();
			ForceReactionTangeantielle = fabs(ForceReactionNormale.Module()*CoeffFrot)*ForceReactionTangeantielle;
		}
		Vector3 ForceReaction = ForceReactionNormale + ForceReactionTangeantielle;
		
		reaction.SetForce( ForceReaction );
		reaction.SetPoint( ContactPoint);

		_pBox->AddVariableForce( reaction );
	}	
}

// -------------------------------------------------------------------------------------------------------	
// Name : GererSphereBoxCollision
// Goal : Gere le comportement d'une boite et d'une sphere lorsqu'ils collisionnent
// Params : _pBox			: Boite considérée
//				_pSphere	: Sphere considérée
// -------------------------------------------------------------------------------------------------------
void Scene::GererSphereBoxCollision( Sphere* _pSphere, Box* _pBox)
{
	// ---------------------------------- Objet A --------------------------------------------------------------
	Vector3	vColPtA;							// Coordonnées du point de collision dans le repere de l'objet A
	Vector3	vColPtA_N, vColPtA_T;		// Composante Normale et tangeantielle du point de collision dans le repre de l'objet A
	Vector3	vRotAxeA;						// Axe de rotation de l'objet A
	Vector3	vNormal_A, vTangeante_A;// Vecteurs normale et tangeantielle allant de B vers A
	Vector3	vVitLin_A;					// Vitesse de l'objet A

	float		fInertM_A;						// Moment d'inertie de l'objet A
	float		fMass_A;						// Masse de l'objet A

	// ---------------------------------- Objet B --------------------------------------------------------------
	Vector3	vColPtB;							// Coordonnées du point de collision dans le repere de l'objet B
	Vector3	vColPtB_N, vColPtB_T;		// Composante Normale et tangeantielle du point de collision dans le repre de l'objet B
	Vector3	vColPtB_Perp;					// Vecteur perpendiculaire au contact point
	Vector3	vRotAxeB;						//	Axe de rotation de l'objet B
	Vector3	vR1, vR2, vR3;					// Aces du repere de l'objet B
	Vector3	vNormal_B, vTangeante_B;// Vecteurs normale et tangeantielle allant de A vers B
	Vector3	vVitLin_B;						// Vitesse linéaire de l'objet B
	Vector3	vVitAng_B;						// Vitesse angulaire de l'objet B

	float		fInertM_B;						// Moment d'inertie de l'objet B
	float		fMass_B;						// Masse de l'objet B

	// ---------------------------------- Commun --------------------------------------------------------------
	Vector3	vColPtVit;							// Vitesse du point de collision
	Vector3	vColPtVit_N, vColPtVit_T;		//	Vecteurs normal et tangeantielle de la vitesse du point de collision
	Vector3	vImpulse;
	Vector3	vVitReflect;
	Vector3	vTemp;

	float		fColPtVit_N, fColPtVit_T	;		// Composantes normale et tangeantielle de la vitesse du contact point
	float		fSigne;
	float		fB1, fB2, fB3;
	float		fMUs, fPq, fPd;
	float		fImpulsey, fImpulsex;
	float		fR1Proj, fR2Proj, fR3Proj;
	
	float fCoeffAm = _pSphere->GetMateriau()->GetAmortCoef();
	fCoeffAm *= _pBox->GetMateriau()->GetAmortCoef();
	float fCoeffFr = _pSphere->GetMateriau()->GetFrottementGlissementCoef();
	fCoeffFr *= _pBox->GetMateriau()->GetFrottementGlissementCoef();

	//---


	//-----------------------------------------------------------------------------------------------
	// Calculs préalables
	//-----------------------------------------------------------------------------------------------

	// Calculs de la normale
	vNormal_A = FindNormal( _pSphere, _pBox);		// Normale orientée de la box vers la sphère
	vNormal_B = - vNormal_A;

	//-----------------------------------------------------------------------------------------------
	// Calcul des variables pour l'objet A
	//-----------------------------------------------------------------------------------------------
	
	// Find collision point
	vColPtA = - _pSphere->GetRayon() * vNormal_A;

	// Calcul des composantes du point du contact
	vColPtA_N	 =	( vColPtA * vNormal_A ) * vNormal_A; 
	vColPtA_T = vColPtA - vColPtA_N;
	
	// Calcul de l'axe de rotation
	vRotAxeA = vColPtA.ProdVectoriel( vNormal_A );
	vRotAxeA.Normaliser();

	// Calcul des moments d'inertie
	fInertM_A	=	0;			// TODO : si on fait tourner la sphere...
	
	if( fInertM_A == 0.0f )
		fInertM_A = 1000000000.f; 

	fMass_A = _pSphere->GetMass();

	// Vitesse de l'objet A
	if( _pSphere->IsInCollision() == true )
		vVitLin_A = _pSphere->GetColVelocity();
	else
		vVitLin_A = _pSphere->GetNewVelocity();


	//-----------------------------------------------------------------------------------------------
	// Calcul des variables pour l'objet B
	//-----------------------------------------------------------------------------------------------
	vR1	=	_pBox->GetR1();
	vR2	=	_pBox->GetR2();
	vR3	=	_pBox->GetR3();

	fR1Proj = vR1 * vNormal_B;
	fR2Proj = vR2 * vNormal_B;
	fR3Proj = vR3 * vNormal_B;

	// Find collision point in B reper ( 26 points de contacts possibles )
	if( fabs( fR1Proj ) >= 0.2f )					
	{
		if ( fR1Proj < 0.f )
			vColPtB = _pBox->GetDim1() / 2 * vR1;
		else
			vColPtB = -_pBox->GetDim1() / 2 * vR1;
	}
	if( fabs( fR2Proj ) >= 0.2f  )					
	{
		if ( fR2Proj < 0.f )
			vColPtB += _pBox->GetDim2() / 2 * vR2;
		else
			vColPtB -= _pBox->GetDim1() / 2 * vR2;
	}	
	if( fabs( fR3Proj ) >= 0.2f  )				
	{
		if ( fR3Proj < 0.f )
			vColPtB += _pBox->GetDim3() / 2 * vR3;
		else
			vColPtB -= -_pBox->GetDim3() / 2 * vR3;
	}

	// Calcul des composantes du point du contact
	vColPtB_N	 =	( vColPtB * vNormal_B ) * vNormal_B; 
	vColPtB_T = vColPtB - vColPtB_N;
	
	// Calcul de l'axe de rotation
	vRotAxeB = vColPtB.ProdVectoriel( vNormal_B );
	vRotAxeB.Normaliser();

	// Calcul du moment d'inertie
	fInertM_B		=	( _pBox->GetInertieMatrix() * vRotAxeB ).Module();

	if( fInertM_B == 0.0f )
		fInertM_B = 1000000000.f; 

	fMass_B = _pBox->GetMass();

	// Vitesse de l'objet B
	if( _pBox->IsInCollision() == true )
		vVitLin_B = _pBox->GetColVelocity();
	else
		vVitLin_B = _pBox->GetNewVelocity();
	//---------------------------------------------------------


	//-----------------------------------------------------------------------------------------------
	// Calcul des variables communes
	//-----------------------------------------------------------------------------------------------

	// Vitesse du point de collision
	vColPtVit =		vVitLin_A;
	vColPtVit -=	vVitLin_B + _pBox->GetVitesseAngulaire().ProdVectoriel( vColPtB );	

	//Classification des mouvements
	if( vColPtA_T.Module() != 0 )
		vTangeante_A = - vColPtA_T.Normalised();
	else
		vTangeante_A = ( vColPtVit - ( vColPtVit * vNormal_A ) * vNormal_A ).Normalised();

	vTangeante_B = - vTangeante_A;

	// Calculs des composantes normale et tangeantielle de la vitesse au point de collision
	fColPtVit_N = (vColPtVit * vNormal_A ); 
	fColPtVit_T = (vColPtVit - fColPtVit_N * vNormal_A) * vTangeante_A; 

	// Recherche du signe de la composante tangeantielle
	if( fColPtVit_T == 0)
		fSigne = 1;
	else
		fSigne = fColPtVit_T / fabs( fColPtVit_T );
	//---------------------------------------------------------


	//-----------------------------------------------------------------------------------------------
	// Calcul des coefficients
	//-----------------------------------------------------------------------------------------------
	fB1 = 1 / fMass_A + 1 / fMass_B + vColPtA_N.ModuleCarre() / fInertM_A + vColPtB_N.ModuleCarre() / fInertM_B;
	fB2 = 1 / fMass_A + 1 / fMass_B + vColPtA_T.ModuleCarre() / fInertM_A + vColPtB_T.ModuleCarre() / fInertM_B;
	fB3 = ( vColPtA_N.Module() * vColPtA_T.Module() ) / fInertM_A + ( vColPtB_N.Module() * vColPtB_T.Module() ) / fInertM_B ;
	
	fMUs	= fB3 / fB1;
	fPd	= ( fB2 + fSigne * fCoeffFr * fB3) * fSigne * fColPtVit_T;
	fPq	= ( fCoeffFr * fB1 + fSigne * fB3 ) * fabs( fColPtVit_N );
	//---------------------------------------------------------

	
	//-----------------------------------------------------------------------------------------------
	// Calcul de l'impulse
	//-----------------------------------------------------------------------------------------------

	// Glissement
	if( fPd > ( 1 + fCoeffAm ) * fPq ) 
	{
		fImpulsey = - ( 1 + fCoeffAm ) * fColPtVit_N / ( fB2 + fSigne * fCoeffFr * fB3 );
		fImpulsex = - fSigne * fCoeffFr * fImpulsey;
	}

	// R-adherence
	if( ( fPq < fPd)		&&		( fPd < ( 1 + fCoeffAm ) * fPq)	 && ( fCoeffFr > fMUs)  )
	{
		fImpulsey = -( 1 + fCoeffAm ) * fColPtVit_N / ( fB2 + fSigne * fCoeffFr * fB3);
		fImpulsex = ( fB3 * fImpulsey - fColPtVit_T) / fB1;
	}

	//R-glissement
	if( ( fPq < fPd)		&&	 ( fPd < ( 1 + fCoeffAm ) * fPq)	&&		( fCoeffFr <= fMUs) )
	{
		fImpulsey = - ( 1 + fCoeffAm ) * fColPtVit_N / ( fB2 + fSigne * fCoeffFr * fB3 );
		fImpulsex = fSigne * fCoeffFr * ( fImpulsey - 2 * fColPtVit_T / ( fB3 + fSigne * fCoeffFr * fB1 ) );
	}

	//C-adherence
	if( ( fPd < fPq)		&&	 ( fCoeffFr > fMUs) )
	{
		fImpulsey = - ( 1 + fCoeffAm ) * ( fB1 * fColPtVit_N + fB3 * fColPtVit_T ) / ( fB1 * fB2 - fB3 * fB3 );
		fImpulsex = ( fB3 * fImpulsey - fColPtVit_T) / fB1; 
	}

	//C-glissement
	if( ( fPd < fPq)		&&		 ( fCoeffFr <= fMUs ) )
	{
		fImpulsey = fColPtVit_N + 2 * fSigne * fCoeffFr * fB3 * fColPtVit_T / ( fB3 + fSigne * fCoeffFr * fB1);
		fImpulsey *= -( 1 + fCoeffAm ) / ( fB2 - fSigne * fCoeffFr * fB3 );
		fImpulsex = fSigne * fCoeffFr * ( fImpulsey - 2 * fColPtVit_T / ( fB3 + fSigne * fCoeffFr * fB1));
	}

	vImpulse = fImpulsey * vNormal_A + fImpulsex * vTangeante_A;
	//---------------------------------------------------------


	//-----------------------------------------------------------------------------------------------
	// Influence de l'impulse sur l'objet A
	//-----------------------------------------------------------------------------------------------
	if( vVitLin_A * vNormal_A < 0 )
	{
		vVitReflect = vVitLin_A + vImpulse / fMass_A;
	}
	else
		vVitReflect = vVitLin_A;

	_pSphere->SetCollisionPosition( _pSphere->GetNewPos() );
	_pSphere->SetCollision( vVitReflect , 0.0f );
	//---------------------------------------------------------	


	//-----------------------------------------------------------------------------------------------
	// Influence de l'impulse sur l'objet B
	//-----------------------------------------------------------------------------------------------
//	if( vVitLin_B * vNormal_B < 0 )
//	{
		vVitReflect = vVitLin_B - vImpulse / fMass_B;
//	}
//	else
//	vVitReflect = vVitLin_B;

	// Calculs des nouvelles vitesses
	vColPtB_Perp	=  - vColPtB_T.Module() * vColPtB_N.Normalised() + vColPtB_N.Module() * vColPtB_T.Normalised();
	vVitAng_B		= ( fabs( vColPtB_Perp * - vImpulse ) / fInertM_B) * ( vColPtB.ProdVectoriel( vNormal_B).Normalised() );
	vVitAng_B		+=	_pBox->GetVitesseAngulaire(); 

	// MAJ des vitesses
	_pBox->SetVitesseAngulaire( vVitAng_B );
	_pBox->SetCollisionPosition(_pBox->GetNewPos(), 0.0f );
	_pBox->SetCollision( vVitReflect , 0.0f );
	//---------------------------------------------------------

	//TODO rajout des forces de réactions quand les objets sont à l'arrêt et en contact sinon ils passent a travers

}

// -------------------------------------------------------------------------------------------------------	
// Name : GererBoxBoxCollision
// Goal : Gere le comportement d'une boite et d'une sphere lorsqu'ils collisionnent
// Params : _pBox1			: Boite1 considérée
//			_pBox2	: Boite2 considérée
// -------------------------------------------------------------------------------------------------------
void Scene::GererBoxBoxCollision( Box* _pBox1, Box* _pBox2, Vector3 vNormal, Vector3 vContactPoint)
{
	// ---------------------------------- Objet A --------------------------------------------------------------
	Vector3	vColPtA;							// Coordonnées du point de collision dans le repere de l'objet A
	Vector3	vColPtA_N, vColPtA_T;		// Composante Normale et tangeantielle du point de collision dans le repre de l'objet A
	Vector3	vColPtA_Perp;					// Vecteur perpendiculaire au contact point
	Vector3	vRotAxeA;						// Axe de rotation de l'objet A
	Vector3	vNormal_A, vTangeante_A;// Vecteurs normale et tangeantielle allant de B vers A
	Vector3	vVitLin_A;					// Vitesse de l'objet A
	Vector3	vVitAng_A;						// Vitesse angulaire de l'objet A


	float		fInertM_A;						// Moment d'inertie de l'objet A
	float		fMass_A;						// Masse de l'objet A

	// ---------------------------------- Objet B --------------------------------------------------------------
	Vector3	vColPtB;							// Coordonnées du point de collision dans le repere de l'objet B
	Vector3	vColPtB_N, vColPtB_T;		// Composante Normale et tangeantielle du point de collision dans le repre de l'objet B
	Vector3	vColPtB_Perp;					// Vecteur perpendiculaire au contact point
	Vector3	vRotAxeB;						//	Axe de rotation de l'objet B
	Vector3	vR1, vR2, vR3;					// Aces du repere de l'objet B
	Vector3	vNormal_B, vTangeante_B;// Vecteurs normale et tangeantielle allant de A vers B
	Vector3	vVitLin_B;						// Vitesse linéaire de l'objet B
	Vector3	vVitAng_B;						// Vitesse angulaire de l'objet B

	float		fInertM_B;						// Moment d'inertie de l'objet B
	float		fMass_B;						// Masse de l'objet B

	// ---------------------------------- Commun --------------------------------------------------------------
	Vector3	vColPtVit;							// Vitesse du point de collision
	Vector3	vColPtVit_N, vColPtVit_T;		//	Vecteurs normal et tangeantielle de la vitesse du point de collision
	Vector3	vImpulse;
	Vector3	vVitReflect;
	Vector3	vTemp;

	float		fColPtVit_N, fColPtVit_T	;		// Composantes normale et tangeantielle de la vitesse du contact point
	float		fSigne;
	float		fB1, fB2, fB3;
	float		fMUs, fPq, fPd;
	float		fImpulsey, fImpulsex;
	
	// Temp	
	float fCoeffFr = _pBox1->GetMateriau()->GetFrottementGlissementCoef();
	fCoeffFr *= _pBox2->GetMateriau()->GetFrottementGlissementCoef();
	float fCoeffAm = _pBox1->GetMateriau()->GetAmortCoef();
	fCoeffAm *= _pBox2->GetMateriau()->GetAmortCoef();
	//---


	//-----------------------------------------------------------------------------------------------
	// Calculs préalables
	//-----------------------------------------------------------------------------------------------

	// Calculs de la normale
	vNormal_A = vNormal;		// Normale orientée 
	vNormal_B = - vNormal_A;

	//-----------------------------------------------------------------------------------------------
	// Calcul des variables pour l'objet A
	//-----------------------------------------------------------------------------------------------
	
	// Find collision point
	vColPtA = vContactPoint - _pBox1->GetNewPos();

	// Calcul des composantes du point du contact
	vColPtA_N	 =	( vColPtA * vNormal_A ) * vNormal_A; 
	vColPtA_T = vColPtA - vColPtA_N;
	
	// Calcul de l'axe de rotation
	vRotAxeA = vColPtA.ProdVectoriel( vNormal_A );
	vRotAxeA.Normaliser();

	// Calcul des moments d'inertie
	fInertM_A	=	( _pBox1->GetInertieMatrix() * vRotAxeA ).Module();
	
	if( fInertM_A == 0.0f )
		fInertM_A = 1000000000.f; 

	fMass_A = _pBox1->GetMass();

	// Vitesse de l'objet A
	if( _pBox1->IsInCollision() == true )
		vVitLin_A = _pBox1->GetColVelocity();
	else
		vVitLin_A = _pBox1->GetNewVelocity();


	//-----------------------------------------------------------------------------------------------
	// Calcul des variables pour l'objet B
	//-----------------------------------------------------------------------------------------------

	// Find collision point
	vColPtB = vContactPoint - _pBox2->GetNewPos();

	// Calcul des composantes du point du contact
	vColPtB_N	 =	( vColPtB * vNormal_B ) * vNormal_B; 
	vColPtB_T = vColPtB - vColPtB_N;
	
	// Calcul de l'axe de rotation
	vRotAxeB = vColPtB.ProdVectoriel( vNormal_B );
	vRotAxeB.Normaliser();

	// Calcul du moment d'inertie
	fInertM_B		=	( _pBox2->GetInertieMatrix() * vRotAxeB ).Module();

	if( fInertM_B == 0.0f )
		fInertM_B = 1000000000.f; 

	fMass_B = _pBox2->GetMass();

	// Vitesse de l'objet B
	if( _pBox2->IsInCollision() == true )
		vVitLin_B = _pBox2->GetColVelocity();
	else
		vVitLin_B = _pBox2->GetNewVelocity();
	//---------------------------------------------------------


	//-----------------------------------------------------------------------------------------------
	// Calcul des variables communes
	//-----------------------------------------------------------------------------------------------

	// Vitesse du point de collision
	vColPtVit =		vVitLin_A + _pBox1->GetVitesseAngulaire().ProdVectoriel( vColPtA );
	vColPtVit -=	vVitLin_B + _pBox2->GetVitesseAngulaire().ProdVectoriel( vColPtB );	

	//Classification des mouvements
	if( vColPtA_T.Module() != 0 )
		vTangeante_A = - vColPtA_T.Normalised();
	else
		vTangeante_A = ( vColPtVit - ( vColPtVit * vNormal_A ) * vNormal_A ).Normalised();

	vTangeante_B = - vTangeante_A;

	// Calculs des composantes normale et tangeantielle de la vitesse au point de collision
	fColPtVit_N = (vColPtVit * vNormal_A ); 
	fColPtVit_T = (vColPtVit - fColPtVit_N * vNormal_A) * vTangeante_A; 

	// Recherche du signe de la composante tangeantielle
	if( fColPtVit_T == 0)
		fSigne = 1;
	else
		fSigne = fColPtVit_T / fabs( fColPtVit_T );
	//---------------------------------------------------------


	//-----------------------------------------------------------------------------------------------
	// Calcul des coefficients
	//-----------------------------------------------------------------------------------------------
	fB1 = 1 / fMass_A + 1 / fMass_B + vColPtA_N.ModuleCarre() / fInertM_A + vColPtB_N.ModuleCarre() / fInertM_B;
	fB2 = 1 / fMass_A + 1 / fMass_B + vColPtA_T.ModuleCarre() / fInertM_A + vColPtB_T.ModuleCarre() / fInertM_B;
	fB3 = ( vColPtA_N.Module() * vColPtA_T.Module() ) / fInertM_A + ( vColPtB_N.Module() * vColPtB_T.Module() ) / fInertM_B ;
	
	fMUs	= fB3 / fB1;
	fPd	= ( fB2 + fSigne * fCoeffFr * fB3) * fSigne * fColPtVit_T;
	fPq	= ( fCoeffFr * fB1 + fSigne * fB3 ) * fabs( fColPtVit_N );
	//---------------------------------------------------------

	
	//-----------------------------------------------------------------------------------------------
	// Calcul de l'impulse
	//-----------------------------------------------------------------------------------------------

	// Glissement
	if( fPd > ( 1 + fCoeffAm ) * fPq ) 
	{
		fImpulsey = - ( 1 + fCoeffAm ) * fColPtVit_N / ( fB2 + fSigne * fCoeffFr * fB3 );
		fImpulsex = - fSigne * fCoeffFr * fImpulsey;
	}

	// R-adherence
	if( ( fPq < fPd)		&&		( fPd < ( 1 + fCoeffAm ) * fPq)	 && ( fCoeffFr > fMUs)  )
	{
		fImpulsey = -( 1 + fCoeffAm ) * fColPtVit_N / ( fB2 + fSigne * fCoeffFr * fB3);
		fImpulsex = ( fB3 * fImpulsey - fColPtVit_T) / fB1;
	}

	//R-glissement
	if( ( fPq < fPd)		&&	 ( fPd < ( 1 + fCoeffAm ) * fPq)	&&		( fCoeffFr <= fMUs) )
	{
		fImpulsey = - ( 1 + fCoeffAm ) * fColPtVit_N / ( fB2 + fSigne * fCoeffFr * fB3 );
		fImpulsex = fSigne * fCoeffFr * ( fImpulsey - 2 * fColPtVit_T / ( fB3 + fSigne * fCoeffFr * fB1 ) );
	}

	//C-adherence
	if( ( fPd < fPq)		&&	 ( fCoeffFr > fMUs) )
	{
		fImpulsey = - ( 1 + fCoeffAm ) * ( fB1 * fColPtVit_N + fB3 * fColPtVit_T ) / ( fB1 * fB2 - fB3 * fB3 );
		fImpulsex = ( fB3 * fImpulsey - fColPtVit_T) / fB1; 
	}

	//C-glissement
	if( ( fPd < fPq)		&&		 ( fCoeffFr <= fMUs ) )
	{
		fImpulsey = fColPtVit_N + 2 * fSigne * fCoeffFr * fB3 * fColPtVit_T / ( fB3 + fSigne * fCoeffFr * fB1);
		fImpulsey *= -( 1 + fCoeffAm ) / ( fB2 - fSigne * fCoeffFr * fB3 );
		fImpulsex = fSigne * fCoeffFr * ( fImpulsey - 2 * fColPtVit_T / ( fB3 + fSigne * fCoeffFr * fB1));
	}

	vImpulse = fImpulsey * vNormal_A + fImpulsex * vTangeante_A;
	//---------------------------------------------------------


	//-----------------------------------------------------------------------------------------------
	// Influence de l'impulse sur l'objet A
	//-----------------------------------------------------------------------------------------------
	if( vVitLin_A * vNormal_A < 0 )
	{
		vVitReflect = vVitLin_A + vImpulse / fMass_A;
	}
	else
		vVitReflect = vVitLin_A;

	// Calculs des nouvelles vitesses
	vColPtA_Perp	=  - vColPtA_T.Module() * vColPtA_N.Normalised() + vColPtA_N.Module() * vColPtA_T.Normalised();
	vVitAng_A		= ( fabs( vColPtA_Perp * - vImpulse ) / fInertM_A) * ( vColPtA.ProdVectoriel( vNormal_A).Normalised() );
	vVitAng_A		+=	_pBox1->GetVitesseAngulaire(); 

	_pBox1->SetVitesseAngulaire( vVitAng_A );
	_pBox1->SetCollisionPosition( _pBox1->GetNewPos(), 0.0f );
	_pBox1->SetCollision( vVitReflect , 0.0f );
	//---------------------------------------------------------	


	//-----------------------------------------------------------------------------------------------
	// Influence de l'impulse sur l'objet B
	//-----------------------------------------------------------------------------------------------
//	if( vVitLin_B * vNormal_B < 0 )
//	{
		vVitReflect = vVitLin_B - vImpulse / fMass_B;
//	}
//	else
//	vVitReflect = vVitLin_B;

	// Calculs des nouvelles vitesses
	vColPtB_Perp	=  - vColPtB_T.Module() * vColPtB_N.Normalised() + vColPtB_N.Module() * vColPtB_T.Normalised();
	vVitAng_B		= ( fabs( vColPtB_Perp * - vImpulse ) / fInertM_B) * ( vColPtB.ProdVectoriel( vNormal_B).Normalised() );
	vVitAng_B		+=	_pBox2->GetVitesseAngulaire(); 

	// MAJ des vitesses
	_pBox2->SetVitesseAngulaire( vVitAng_B );
	_pBox2->SetCollisionPosition(_pBox2->GetNewPos(), 0.0f );
	_pBox2->SetCollision( vVitReflect , 0.0f );
	//---------------------------------------------------------

	//TODO rajout des forces de réactions quand les objets sont à l'arrêt et en contact sinon ils passent a travers

}

float Scene::FindDeltaT(Box* _pBox, Plan* _pPlan, Vector3 ContactPoint, Vector3 BoxVelocity)
{
	return -2; //on se passe du recalage pour l'instant
/*	Vector3 vNormal = _pPlan->GetNormal();
	float fBoxPlanD = 	fabs((_pBox->GetNewPos() - _pPlan->GetPos())*vNormal);
	Vector3 vContact = ContactPoint;
	float Phi = acosf(fabs(vContact*vNormal)/vContact.Module());
	float vVAngulaire = (_pBox->GetVitesseAngulaire()).Module();
	float a = fabs(BoxVelocity*vNormal)/vContact.Module();
	float b = fBoxPlanD/vContact.Module();
	float DT;
	Vector3 VelocityNorme = BoxVelocity;
	VelocityNorme.Normaliser();


	if( vVAngulaire <= 0.2f && BoxVelocity.Module() <=0.2f && (fBoxPlanD - fabs(ContactPoint*vNormal))> -0.01f )
	{
		_pBox->SetCollision(Vector3(0.f,0.f,0.f) , 0.0f );
		_pBox->SetVitesseAngulaire(Vector3(0.0f,0.0f,0.0f));
		return -1.0f;
	}

	if( vVAngulaire <= 0.2f && BoxVelocity.Module() <=0.2f ) //petite vitesse mais objet trop enfoncé
		return -2.0f;
	
	if( fabs(VelocityNorme*vNormal)<0.3f ) //Pas de vitesse normale
		return -2.0f;

	// Cas d'une petite vitesse angulaire
	if ( vVAngulaire <= 0.5f )
	{
		DT = (fabs(ContactPoint*vNormal)-fBoxPlanD)/fabs(BoxVelocity*vNormal);
		return DT;
	}

	// Recherche dichotomique
	float BorneSup = (PI/2-Phi)/vVAngulaire;
	float BorneInf = 0.0f;

	if( BorneSup > 1.0f)
		BorneSup = 1.0f;

	for(int i=0;i<10;i++)
	{
		float DichoDT = BorneInf+(BorneSup - BorneInf)/2;
		float terme1 = cos(vVAngulaire*DichoDT+Phi);
		float terme2 = -(a*DichoDT+b);
		if(terme1+terme2>0)
			BorneInf = DichoDT;
		else if(terme1+terme2<0)
			BorneSup = DichoDT;
		else
		{
			BorneSup = DichoDT;
			break;
		}
	}
	return BorneInf;*/
}


// -------------------------------------------------------------------------------------------------------	
// Name : FindNormal
// Goal : Permet de calculer la normale de la collision entre la boite et la sphere
// Params : _pBox			: Boite considérée
//				_pSphere	: Sphere considérée
// -------------------------------------------------------------------------------------------------------
Vector3	Scene::FindNormal( Sphere* _pSphere, Box* _pBox )
{
	Vector3	vBoxToSph;
	
	float		fProjR1, fProjR2, fProjR3;
	float		fSizeR1, fSizeR2, fSizeR3;

	vBoxToSph = _pSphere->GetNewPos() -  _pBox->GetNewPos();

	// On projette ce vecteur sur les axes du cube 
	fProjR1	=	vBoxToSph * _pBox->GetR1();
	fProjR2	=	vBoxToSph * _pBox->GetR2();
	fProjR3	=	vBoxToSph * _pBox->GetR3();

	// Recupération des dimensions du cube
	fSizeR1 = _pBox->GetDim1	() * 0.5f;
	fSizeR2 = _pBox->GetDim2	() * 0.5f;
	fSizeR3 = _pBox->GetDim3	() * 0.5f;

	// Si la sphere tappe sur une face, on recherche laquelle

	// Sur la face gauche ou droite
	if ( fabs(fProjR2) < fSizeR2		&&		fabs(fProjR3) < fSizeR3 )
	{
		if ( fProjR1 > fSizeR1 )	// face droite
			return _pBox->GetR1();
		else											// face gauche
			return -_pBox->GetR1();
	}

	// Sur la face dessus ou dessous
	if ( fabs(fProjR1) < fSizeR1		&&		fabs(fProjR3) < fSizeR3 )
	{
		if ( fProjR2 > fSizeR2 )	// face dessus
			return _pBox->GetR2();
		else											// face dessous
			return -_pBox->GetR2();
	}

	// Sur la face devant ou derriere
	if ( fabs(fProjR1) < fSizeR1		&&		fabs(fProjR2) < fSizeR2 )
	{
		if ( fProjR3 > fSizeR3 )	// face derriere
			return _pBox->GetR3();
		else											// face devant
			return -_pBox->GetR3();
	}

	// Sinon la sphere tappe sur un "angle" de la box
	return vBoxToSph.Normalised();
}

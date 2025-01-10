/*****************************************************************************
 *
 * File			: NtlSobProjectile.h
 * Author		: HyungSuk, Jang
 * Copyright	: (¡÷)NTL
 * Date			: 2005. 12. 08	
 * Abstract		: Simulation projectile object
 *****************************************************************************
 * Desc         : As a projectile object, it corresponds to arrows, bombs, missiles, and magic projectiles.
 *                It has a simple state machine.
                  I remember my owner serial.
				  Projectiles have range attack, target attack, and penetration styles, and in the case of target attack, 
				  You can also follow the target.
 *****************************************************************************/

#ifndef __NTL_SOB_PROJECTILE_H__
#define __NTL_SOB_PROJECTILE_H__

#include "NtlSobActor.h"
#include "NtlBehaviorData.h"

class CNtlSobActor;
class CNtlBehaviorBase;

class CNtlSobProjectile : public CNtlSobActor
{
	DECLEAR_MEMORY_POOL(CNtlSobProjectile, NTL_DEFAULT_MEMORY_POOL)
			
protected:

	// attribute
	RwReal	m_fSpeed;								/** speed */
	RwBool	m_bLeftHand;							/** left hand */
	RwV3d	m_vDest;								/** dest position */
	RwReal	m_fTargetLocHeight;						/** target position offset */
	std::list<SHitStuff> m_listHitStuff;			/** hit list */
	std::list<CNtlBehaviorBase*> m_listBehavior;	/** behavior list */

	RwBool	m_bDamageProc;							/** projectile¿« damage procedure */
	RwBool	m_bFinish;								/** projectile¿« life flag */

	
	// transform
	RwMatrix m_matTrans;

protected:

	// update
	RwBool UpdateBehavior(RwReal fElapsed);
	void UpdateVisual(void);
	
	// event handler
	void CreateEventHandler(RWS::CMsg &pMsg);

	// damage proc
	void DamageProc(void);
	
public:

	CNtlSobProjectile();
	virtual ~CNtlSobProjectile();

	virtual RwBool Create(void);

	virtual void Destroy(void);

	virtual CNtlFSMLayer* CreateFSM(void);

	virtual CNtlSobInfluence* CreateSobInfluence(void);

	virtual void Update(RwReal fElapsed);

	virtual RwBool IsFinish(void);

	virtual void HandleEvents(RWS::CMsg &pMsg);
	
public:

	void SetSpeed(RwReal fSpeed);
	RwReal GetSpeed(void) const;

	void SetLeftHand(RwBool bLeftHand);
	RwBool GetLeftHand(void) const;

	void SetDestPositin(const RwV3d *pDest);
	RwV3d GetDestPosition(void);

	void AddHit(SHitStuff& sHit);
	RwInt32 GetHitCount(void) const;
	RwReal GetHitTargetHeight(void);

	void AddBehavior(CNtlBehaviorBase *pBehavior);
	RwInt32 GetBehaviorCount(void) const;
};

inline void CNtlSobProjectile::SetSpeed(RwReal fSpeed)
{
	m_fSpeed = fSpeed;
}

inline RwReal CNtlSobProjectile::GetSpeed(void) const
{
	return m_fSpeed;
}

inline void CNtlSobProjectile::SetLeftHand(RwBool bLeftHand)
{
	m_bLeftHand = bLeftHand;
}

inline RwBool CNtlSobProjectile::GetLeftHand(void) const
{
	return m_bLeftHand;
}

inline void CNtlSobProjectile::SetDestPositin(const RwV3d *pDest)
{
	RwV3dAssignMacro(&m_vDest, pDest);
}

inline RwV3d CNtlSobProjectile::GetDestPosition(void)
{
	return m_vDest;
}


#endif

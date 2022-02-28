/***
* Weapon code written by: https://www.moddb.com/members/johncalhoun
* 
* Additional fixes and lines written by: https://github.com/JackBailey
****/

#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "monsters.h"
#include "weapons.h"
#include "nodes.h"
#include "player.h"
// jay - removed gamerules include

// jay - removed shotgun spreads, xash is SP only anyways

enum aa12_e {
	AA12_IDLE = 0,
	AA12_SHOOT1,
	AA12_SHOOT2,
	AA12_INSERT,
	AA12_AFTER_RELOAD,
	AA12_START_RELOAD,
	AA12_DRAW,
};

class CAA12 : public CBasePlayerWeapon
{
public:
	void Spawn(void);
	void Precache(void);
	int iItemSlot() { return 3; }
	int GetItemInfo(ItemInfo* p);
	int AddToPlayer(CBasePlayer* pPlayer);

	void PrimaryAttack(void);
	BOOL Deploy();
	void Reload(void);
	void WeaponIdle(void);

	//int m_fInReload;
	//float m_flNextReload;
	int m_iShell;
	//float m_flPumpTime;
	// jay - these commented out lines are unused in an AA12. they are used for the shotgun's custom reload and pump sound
};
LINK_ENTITY_TO_CLASS(weapon_aa12, CAA12);


void CAA12::Spawn()
{
	Precache();
	m_iId = WEAPON_AA12;
	SET_MODEL(ENT(pev), "models/w_autoass.mdl");

	m_iDefaultAmmo = AA12_DEFAULT_GIVE;

	FallInit();// get ready to fall
}


void CAA12::Precache(void)
{
	PRECACHE_MODEL("models/v_autoass.mdl");
	PRECACHE_MODEL("models/w_autoass.mdl");
	PRECACHE_MODEL("models/p_autoass.mdl");

	m_iShell = PRECACHE_MODEL("models/shotgunshell.mdl");// shotgun shell

	//PRECACHE_SOUND("items/9mmclip1.wav");
	// jay - ammo pickup sound; not needed in precache

	PRECACHE_SOUND("weapons/auto1.wav");// AA12 fire 1
	PRECACHE_SOUND("weapons/auto2.wav");// AA12 fire 2 

	PRECACHE_SOUND("weapons/clipinsert1.wav");	// AA12 reload
	PRECACHE_SOUND("weapons/cliprelease1.wav");	// AA12 reload

//	PRECACHE_SOUND ("weapons/clipinsert1.wav");	// AA12 reload - played on client
//	PRECACHE_SOUND ("weapons/clipinsert2.wav");	// AA12 reload - played on client

	PRECACHE_SOUND("weapons/357_cock1.wav"); // gun empty sound
}

int CAA12::AddToPlayer(CBasePlayer* pPlayer)
{
	if (CBasePlayerWeapon::AddToPlayer(pPlayer))
	{
		MESSAGE_BEGIN(MSG_ONE, gmsgWeapPickup, NULL, pPlayer->pev);
		WRITE_BYTE(m_iId);
		MESSAGE_END();
		return TRUE;
	}
	return FALSE;
}


int CAA12::GetItemInfo(ItemInfo* p)
{
	p->pszName = STRING(pev->classname);
	p->pszAmmo1 = "buckshot";
	p->iMaxAmmo1 = BUCKSHOT_MAX_CARRY;
	p->pszAmmo2 = NULL;
	p->iMaxAmmo2 = -1;
	p->iMaxClip = AA12_MAX_CLIP;
	p->iSlot = 3;
	p->iPosition = 4;
	p->iFlags = 0;
	p->iId = m_iId = WEAPON_AA12;
	p->iWeight = AA12_WEIGHT;

	return 1;
}



BOOL CAA12::Deploy()
{
	return DefaultDeploy("models/v_autoass.mdl", "models/p_autoass.mdl", AA12_DRAW, "shotgun");	// jay - the last line here is the player animation. this should be "shotgun" here, as "aa12" doesn't exist
}


void CAA12::PrimaryAttack()
{
	// don't fire underwater
	if (m_pPlayer->pev->waterlevel == 3)
	{
		PlayEmptySound();
		m_flNextPrimaryAttack = gpGlobals->time + 0.2;
		return;
	}

	if (m_iClip <= 0)
	{
		Reload();
		if (m_iClip == 0)
			PlayEmptySound();
		return;
	}

	m_pPlayer->m_iWeaponVolume = LOUD_GUN_VOLUME;
	m_pPlayer->m_iWeaponFlash = NORMAL_GUN_FLASH;

	m_iClip--;
	m_pPlayer->pev->effects = (int)(m_pPlayer->pev->effects) | EF_MUZZLEFLASH;

	SendWeaponAnim(AA12_SHOOT1);

	// player "shoot" animation
	m_pPlayer->SetAnimation(PLAYER_ATTACK1);

	UTIL_MakeVectors(m_pPlayer->pev->v_angle + m_pPlayer->pev->punchangle);

	Vector	vecShellVelocity = m_pPlayer->pev->velocity
		+ gpGlobals->v_right * RANDOM_FLOAT(50, 70)
		+ gpGlobals->v_up * RANDOM_FLOAT(100, 150)
		+ gpGlobals->v_forward * 25;

	EjectBrass(m_pPlayer->pev->origin + m_pPlayer->pev->view_ofs + gpGlobals->v_up * -12 + gpGlobals->v_forward * 20 + gpGlobals->v_right * 4, vecShellVelocity, pev->angles.y, m_iShell, TE_BOUNCE_SHOTSHELL);

	EMIT_SOUND_DYN(ENT(m_pPlayer->pev), CHAN_WEAPON, "weapons/auto1.wav", RANDOM_FLOAT(0.95, 1.0), ATTN_NORM, 0, 93 + RANDOM_LONG(0, 0x1f));


	Vector vecSrc = m_pPlayer->GetGunPosition();
	Vector vecAiming = m_pPlayer->GetAutoaimVector(AUTOAIM_5DEGREES);

	/*
		if (g_pGameRules->IsDeathmatch())
		{
			// altered deathmatch spread
			m_pPlayer->FireBullets(4, vecSrc, vecAiming, VECTOR_CONE_DM_SHOTGUN, 2048, BULLET_PLAYER_BUCKSHOT, 0);
		}
		else
		{
			// regular old, untouched spread.
			m_pPlayer->FireBullets(6, vecSrc, vecAiming, VECTOR_CONE_10DEGREES, 2048, BULLET_PLAYER_BUCKSHOT, 0);
		}
	*/
	// jay - only use singleplayer spread, this is xash after all
	m_pPlayer->FireBullets(6, vecSrc, vecAiming, VECTOR_CONE_10DEGREES, 2048, BULLET_PLAYER_BUCKSHOT, 0);

	if (!m_iClip && m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] <= 0)
		// HEV suit - indicate out of ammo condition
		m_pPlayer->SetSuitUpdate("!HEV_AMO0", FALSE, 0);

	if (m_iClip != 0)
		m_flNextPrimaryAttack = gpGlobals->time + 0.2;

	/*
		if (m_iClip != 0)
			m_flTimeWeaponIdle = gpGlobals->time + 5.0;
		else
			m_flTimeWeaponIdle = 0.75;
	*/
	// jay - not sure what's going on here; this will just kinda break the idle delay if you ran out of ammo
	m_flTimeWeaponIdle = gpGlobals->time + 5;	// jay - fixed idle timer

	//m_fInReload = 0;
	// jay - unused shotgun variable

	m_pPlayer->pev->punchangle.x -= 5;
}


void CAA12::Reload(void)
{
	DefaultReload(AA12_MAX_CLIP, AA12_START_RELOAD, 1.5);
}



void CAA12::WeaponIdle(void)
{
	ResetEmptySound();

	m_pPlayer->GetAutoaimVector(AUTOAIM_5DEGREES);

	if (m_flTimeWeaponIdle > gpGlobals->time)
		return;

	/*
		int iAnim;
		switch (RANDOM_LONG(0, 1))
		{
		case 0:
			iAnim = AA12_IDLE;
			break;
		}

		SendWeaponAnim(iAnim);
	*/
	// jay - if there's only one idle animation, there's no point in declaring a random switch or even using the iAnim variable
	// also, this RANDOM_LONG can return either 0 or 1, and this only has a case for 0. this means that 50% of the time the idle animation will be empty and broken

	// fixed:
	SendWeaponAnim(AA12_IDLE);

	m_flTimeWeaponIdle = gpGlobals->time + RANDOM_FLOAT(10, 15);// how long till we do this again.
}


/*
class CShotgunAmmo : public CBasePlayerAmmo
{
	void Spawn(void)
	{
		Precache();
		SET_MODEL(ENT(pev), "models/w_shotbox.mdl");
		CBasePlayerAmmo::Spawn();
	}
	void Precache(void)
	{
		PRECACHE_MODEL("models/w_shotbox.mdl");
		PRECACHE_SOUND("items/9mmclip1.wav");
	}
	BOOL AddAmmo(CBaseEntity* pOther)
	{
		if (pOther->GiveAmmo(AMMO_BUCKSHOTBOX_GIVE, "buckshot", BUCKSHOT_MAX_CARRY) != -1)
		{
			EMIT_SOUND(ENT(pev), CHAN_ITEM, "items/9mmclip1.wav", 1, ATTN_NORM);
			return TRUE;
		}
		return FALSE;
	}
};
*/

// jay - this class already exists in shotgun.cpp. redefining it in another file breaks everything horribly

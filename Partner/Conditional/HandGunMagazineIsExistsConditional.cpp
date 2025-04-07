#include "HandGunMagazineIsExistsConditional.h"
#include "Actor/Partner/Partner.h"

bool HandGunMagazineIsExistsConditional::Judge()
{
	auto partner = dynamic_cast<Partner*>(m_common.m_owner);
	return partner->GetMagazineNum(Weapon::WeaponType::HandGun) > 0;
}

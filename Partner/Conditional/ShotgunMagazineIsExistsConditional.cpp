#include "ShotgunMagazineIsExistsConditional.h"
#include "Actor/Partner/Partner.h"

bool ShotgunMagazineIsExistsConditional::Judge()
{
	auto partner = dynamic_cast<Partner*>(m_common.m_owner);
	return partner->GetMagazineNum(Weapon::WeaponType::ShotGun) > 0;
}

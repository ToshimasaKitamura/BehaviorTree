#include "ShotgunBulletIsExistsConditional.h"
#include "Actor/Partner/Partner.h"

bool ShotgunBulletIsExistsConditional::Judge()
{
	auto partner = dynamic_cast<Partner*>(m_common.m_owner);
	return partner->GetBulletNum(Weapon::WeaponType::ShotGun) > 0;
}

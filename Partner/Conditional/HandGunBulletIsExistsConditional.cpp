#include "HandGunBulletIsExistsConditional.h"
#include "Actor/Partner/Partner.h"

bool HandGunBulletIsExistsConditional::Judge()
{
    auto partner = dynamic_cast<Partner*>(m_common.m_owner);
    return partner->GetBulletNum(Weapon::WeaponType::HandGun) > 0;
}

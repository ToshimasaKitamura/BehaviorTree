#include "ReloadShotgunAction.h"
#include "Actor/Partner/Partner.h"

void ReloadShotGunAction::Initialize()
{
    auto powner = dynamic_cast<Partner*>(m_common.m_owner);
    powner->ChangeWeapon(Weapon::WeaponType::ShotGun);
    powner->Reload(Weapon::WeaponType::ShotGun);
}

bool ReloadShotGunAction::Update(float deltaTime)
{
    m_time += deltaTime;
    if (m_time >= 10.0f) return false;
    return true;
}

void ReloadShotGunAction::Finalize()
{
}

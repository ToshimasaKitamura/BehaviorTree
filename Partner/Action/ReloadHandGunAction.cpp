#include "ReloadHandGunAction.h"
#include "Actor/Partner/Partner.h"

void ReloadHandGunAction::Initialize()
{
    auto powner = dynamic_cast<Partner*>(m_common.m_owner);
    powner->ChangeWeapon(Weapon::WeaponType::HandGun);
    powner->Reload(Weapon::WeaponType::HandGun);
}

bool ReloadHandGunAction::Update(float deltaTime)
{
    m_time += deltaTime;
    if (m_time >= 10.0f) return false;
    return true;
}

void ReloadHandGunAction::Finalize()
{

}

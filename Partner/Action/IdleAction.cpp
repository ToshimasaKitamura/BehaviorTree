#include "IdleAction.h"
#include "../Partner.h"
#include "Actor/Player/PlayerMotion.h"

void IdleAction::Initialize()
{
	auto cowner = dynamic_cast<Character*>(m_common.m_owner);
	if(cowner) cowner->ChangeState(0, PlayerMotion::Idle, true);
	auto powner = dynamic_cast<Partner*>(m_common.m_owner);
	powner->ChangeWeapon(Weapon::WeaponType::ShotGun);
}

bool IdleAction::Update(float deltaTime)
{
	return true;
}

void IdleAction::Finalize()
{

}

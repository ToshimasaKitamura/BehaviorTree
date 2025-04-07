#include "IsClosedEnemyConditional.h"
#include "Actor/Zombie/Zombie.h"
#include "Collide/Line.h"
#include "Field/Field.h"

constexpr float floorDistance = 1.0f;

bool IsClosedEnemyConditional::Judge()
{
	auto partnerPos = m_common.m_owner->Transform().position();
	auto enemies = m_common.m_world->FindActorWithTag("Enemy");
	auto CanPassBetween = [=](Actor* actor)
	{
		Line line{ m_common.m_owner->Collider().m_center, actor->Collider().m_center };
		if (m_common.m_world->GetField()->Collide(line)) return false;
		else return true;
	};

	for (const auto& enemy : enemies)
	{
		auto pos = enemy->Transform().position();
		if (std::abs(pos.y - partnerPos.y) > floorDistance) continue;
		auto zombie = dynamic_cast<Zombie*>(enemy);
		if (!zombie) continue;
		if (zombie->IsDeadMotion()) continue;
		if (partnerPos.distance(pos) > m_arrivalDistance) continue;
		if (!CanPassBetween(enemy)) continue;
		return true;
	}
	return false;
}

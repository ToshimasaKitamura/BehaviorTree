#include "IsSeperatedPlayerConditional.h"
#include "Field/Field.h"
#include "Collide/Line.h"

bool IsSeperatedPlayerConditional::Judge()
{
	// プレイヤーを取得
	auto player = m_common.m_world->FindActor("Player");
	if (!player) return false;
	//Line line{ m_common.m_owner->Collider().m_center, player->Collider().m_center };
	//if (m_common.m_world->GetField()->Collide(line)) return false;
	// プレイヤーとの距離
	float distance = m_common.m_owner->TargetDistance(player->Transform().position());
	return distance > m_arrivalDistance;
}

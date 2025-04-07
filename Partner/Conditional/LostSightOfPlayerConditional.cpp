#include "LostSightOfPlayerConditional.h"
#include "Field/Field.h"
#include "Collide/Line.h"

#define PRINT(str, ...){char c[256];sprintf_s(c, str, __VA_ARGS__);OutputDebugStringA(c);}

bool LostSightOfPlayerConditional::Judge()
{
	m_agent = std::make_shared<NavGraphAgent>(m_common.m_owner, m_common.m_world);
	auto player = m_common.m_world->FindActor("Player");
	if (!player) return false;

	m_agent->SetTarget(player);
	m_agent->CalcRoute(player->Transform().position());
	Line line;
	line.m_start = m_common.m_owner->Collider().m_center;
	line.m_end = player->Collider().m_center;

	bool hasWall = m_common.m_world->GetField()->Collide(line);
	bool hasRoute = m_agent->IsFoundRoute();
	return hasWall && !hasRoute;
}

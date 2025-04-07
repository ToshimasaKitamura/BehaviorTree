#include "TeleportAction.h"

void TeleportAction::Initialize()
{
}

bool TeleportAction::Update(float deltaTime)
{
	// プレイヤーを取得
	auto player = m_common.m_world->FindActor("Player");
	auto position = player->Transform().position();
	auto back = -player->Transform().forward();
	m_common.m_owner->Transform().position(position + back * 1.0f);
	return false;
}

void TeleportAction::Finalize()
{
}

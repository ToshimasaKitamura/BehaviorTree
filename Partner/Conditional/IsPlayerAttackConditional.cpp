#include "IsPlayerAttackConditional.h"
#include "Actor/Player/Player.h"

bool IsPlayerAttackConditional::Judge()
{
	auto actorPlayer = m_common.m_world->FindActor("Player");
	auto player = dynamic_cast<Player*>(actorPlayer);
	if (player->isAttack())
	{
		return true;
	}
	else return false;
}

#include "IsReceiveGatherOrderConditional.h"
#include "Actor/Partner/Partner.h"

bool IsReceiveGatherOrderConditional::Judge()
{
	auto partner = dynamic_cast<Partner*>(m_common.m_owner);
	return partner->IsGatherOrder();
}

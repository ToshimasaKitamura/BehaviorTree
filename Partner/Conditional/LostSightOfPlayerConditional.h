#pragma once
#include "AI/BehaviorTree/Node/Conditional.h"
#include "NavigationSystem/NavGraphAgent/NavGraphAgent.h"

class LostSightOfPlayerConditional : public Conditional
{
public:
	// 判断
	// true : 成功判定, false : 失敗判定
	bool Judge() override;
private:
	// プレイヤーをおいかける
	std::shared_ptr<NavGraphAgent> m_agent;
};


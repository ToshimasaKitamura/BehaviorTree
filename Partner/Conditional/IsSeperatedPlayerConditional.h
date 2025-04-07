#pragma once
#include "AI/BehaviorTree/Node/Conditional.h"

class IsSeperatedPlayerConditional : public Conditional
{
public:
	IsSeperatedPlayerConditional(float distance) : m_arrivalDistance{distance}{}
	// 判断
	// true : 成功判定, false : 失敗判定
	bool Judge() override;
private:
	const float m_arrivalDistance;
};


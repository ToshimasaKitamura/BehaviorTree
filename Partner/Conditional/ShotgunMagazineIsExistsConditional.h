#pragma once
#include "AI/BehaviorTree/Node/Conditional.h"

class ShotgunMagazineIsExistsConditional : public Conditional
{
public:
	// 判断
	// true : 成功判定, false : 失敗判定
	bool Judge() override;
};
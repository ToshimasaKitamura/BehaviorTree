#pragma once
#include "AI/BehaviorTree/Node/Action.h"

class HandGunAttackAction : public Action
{
public:
	// 初期化
	void Initialize() override;
	// 更新
	// true : 更新続行, false : 終了処理を呼び出す
	bool Update(float deltaTime) override;
	// 終了
	void Finalize() override;
private:
	// 攻撃する
	bool Attack() const;

	float m_time{ 0.0f };

	bool m_isEnd{ false };
};


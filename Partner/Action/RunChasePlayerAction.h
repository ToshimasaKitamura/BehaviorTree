#pragma once
#include "AI/BehaviorTree/Node/Action.h"
#include "NavigationSystem/NavGraphAgent/NavGraphAgent.h"
#include <memory>

class RunChasePlayerAction : public Action
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
	// 近ければ追いかけない
	bool isClose() const;
	// プレイヤーをおいかける
	std::shared_ptr<NavGraphAgent> m_agent;
};
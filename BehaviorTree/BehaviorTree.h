#pragma once
#include <vector>
#include <memory>
#include "Node/Node.h"
#include "Node//Action.h"
#include "World/IWorld.h"
#include "Actor/Actor.h"

class BehaviorTree
{
public:
	// ルートノードを根とした、ビヘイビアツリーを構築します
	explicit BehaviorTree(const std::shared_ptr<Node> rootNode, IWorld* world, Actor* owner);
	~BehaviorTree() = default;

	// 更新
	void Update(float deltaTime);
	// アクションノードの初期化
	void ResetActionNode();
private:
	// コンディショナルノードを調べ、Tickを走らせるべきかどうかを判断する
	bool ShouldTick();
	// ビヘイビアツリーを走査して、実行すべきノード番号を決める
	void Tick();
	// ノード番号を割り当てる
	void MakeNodeList(std::shared_ptr<Node> node);
private:
	// ルートノード
	std::shared_ptr<Node> m_rootNode = 0;
	// ノードを格納するための配列
	std::vector<std::shared_ptr<Node>> m_nodes;
	// 実行中のアクションノード
	std::shared_ptr<Action> m_actionNode = 0;
	// Tick用タイマー
	float m_timer{ 0.0f };
};
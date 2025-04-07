#include "BehaviorTree.h"
#include "Node/Conditional.h"

#define PRINT(str, ...){char c[256];sprintf_s(c, str, __VA_ARGS__);OutputDebugStringA(c);}

constexpr float tickTime = 30.0f;

BehaviorTree::BehaviorTree(const std::shared_ptr<Node> rootNode, IWorld* world, Actor* owner)
	:m_rootNode(rootNode)
{
	MakeNodeList(rootNode);

	// 共通データ生成
	Node::Common common;
	common.m_world = world;
	common.m_owner = owner;
	Node::RegisterCommonData(common);
	Tick();
}

void BehaviorTree::Update(float deltaTime)
{
	m_timer += deltaTime;
	if (m_timer >= tickTime)
	{
		m_timer = 0.0f;
		
		// Tickを走らせる必要があれば、走らす
		if (!m_actionNode) Tick();
		else if (ShouldTick()) Tick();
	}
	
	if (!m_actionNode) return;
	//else if(!m_actionNode) return;
	//PRINT("(≧▽≦)%d\n", m_actionNode->GetIndex());

	// アクションノードの更新
	if (!m_actionNode->Update(deltaTime))
	{
		m_actionNode->Finalize();		// アクションノードの終了処理
		m_actionNode = nullptr;
	}
}

void BehaviorTree::ResetActionNode()
{
	m_actionNode.reset();
}

bool BehaviorTree::ShouldTick()
{
	// 以下の条件を満たすConditionalNodeを再評価対象とし、監視する
	// actionNodeIndexと兄弟のConditionalNodeのうち、最もノード番号が小さいもののノード番号をxとする。
	// x未満のノード番号をもつConditionalNode

	// 現在実行しているアクションノードのノード番号
	int actionNodeIndex = m_actionNode->GetIndex();
	// 親ノードのポインタ
	auto parent = m_nodes[actionNodeIndex]->GetParent();
	// 最も左側にある(最もノード番号が小さい)ノード
	auto smallestNode = parent->GetChildren().front();
	// 最も左側にある(最もノード番号が小さい)ノードのノード番号
	auto smallestNodeIndex = smallestNode->GetIndex();

	for (int i = 0; i < actionNodeIndex; ++i)
	{
		auto conditionalNode = std::dynamic_pointer_cast<Conditional>(m_nodes[i]);
		if (!conditionalNode) continue;

		if (conditionalNode->Judge()) return true;
	}
	return false;
}

void BehaviorTree::Tick()
{
	int actionNodeIndex = -1;
	int preActionNodeIndex = (m_actionNode) ? m_actionNode->GetIndex() : -1;

	if (m_rootNode->Excute(actionNodeIndex) == Node::State::FoundAction)
	{
		// アクションノードを更新
		m_actionNode = std::dynamic_pointer_cast<Action>(m_nodes[actionNodeIndex]);

		if (m_actionNode->GetIndex() != preActionNodeIndex)
		{
			//PRINT("(>_<)%d\n", m_actionNode->GetIndex());
			// アクションノードの初期化処理
			m_actionNode->Initialize();
		}
		//PRINT("pre:%d, cur:%d\n", preActionNodeIndex, m_actionNode->GetIndex());
	}
}

void BehaviorTree::MakeNodeList(std::shared_ptr<Node> node)
{
	node->AssignIndex(m_nodes.size());
	m_nodes.push_back(node);
	for (auto& node : node->GetChildren())
		MakeNodeList(node);
}

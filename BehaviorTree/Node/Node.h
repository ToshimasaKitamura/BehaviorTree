#pragma once
#include <string>
#include <vector>
#include <memory>
#include "World/IWorld.h"
#include "Actor/Actor.h"
#include "State/StateMachine.h"
#include "Actor/Character/Character.h"
#include "Actor/Weapon/Weapon.h"

class Node :public std::enable_shared_from_this<Node>
{
public:
	// ノードの状態クラス
	enum class State
	{
		Success,
		Failure,
		FoundAction
	};

	// 共通データ
	struct Common
	{
		// 経過時間
		float m_time{ 0.0f };
		// ワールドのポインタ
		IWorld* m_world = 0;
		// BTの持ち主のポインタ
		Actor* m_owner = 0;
	};

public:
	// 仮想デストラクタ
	virtual ~Node() = default;

	// ノードを実行する
	// アクションノードを探し出すのが目的です
	// 引数にアクションノードのインデックスを受け取るint型の参照変数を用意してあります
	virtual State Excute(int& actionNodeIndex) = 0;
	 
	// インデックスを割り当てる
	void AssignIndex(int index);
	// 親をセット
	void SetParent(std::shared_ptr<Node> parentNode);
	// 子ノードを追加します
	void AddChild(std::shared_ptr<Node> childNode);
	// 子ノードを一気に追加します
	void AddChildren(std::vector<std::shared_ptr<Node>> childrenNodes);
	// 共通データを登録します
	static void RegisterCommonData(const Common& commonData);

public:
	// ゲッター
	inline int GetIndex() const { return m_index; }
	inline std::shared_ptr<Node> GetParent() const { return m_parent.lock(); }
	inline std::vector<std::shared_ptr<Node>> GetChildren() const { return m_children; }

protected:
	// 共通データ
	static Common m_common;

private:
	// ノード番号
	int m_index{ -1 };
	// 親ノードのポインタ
	std::weak_ptr<Node> m_parent;
	// 子ノードのポインタ
	std::vector<std::shared_ptr<Node>> m_children;
};
#pragma once
#include "Node.h"

class Action : public Node
{
public:
	virtual ~Action() = default;

	/*----------------------------------------------------------------------------------------------------*/
	// 各アクションノードで定義する必要のあるメソッドです。
	/*----------------------------------------------------------------------------------------------------*/
	// 初期化
	virtual void Initialize() = 0;
	// 更新
	// true : 更新続行, false : 終了処理を呼び出す
	virtual bool Update(float deltaTime) = 0;
	// 終了
	virtual void Finalize() = 0;
	/*----------------------------------------------------------------------------------------------------*/

	// 実行時に自身のノードインデックスを設定する
	inline State Excute(int& actionNodeIndex) override 
	{
		actionNodeIndex = GetIndex();
		return State::FoundAction;
	}
};
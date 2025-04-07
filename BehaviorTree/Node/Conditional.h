#pragma once
#include "Node.h"

class Conditional : public Node
{
public:
	virtual ~Conditional() = default;

	/*----------------------------------------------------------------------------------------------------*/
	// 各コンディショナルノードで定義する必要のあるメソッドです。
	/*----------------------------------------------------------------------------------------------------*/
	// 判断
	// true : 成功判定, false : 失敗判定
	virtual bool Judge() = 0;
	/*----------------------------------------------------------------------------------------------------*/

	// Judgeメソッドで判定した結果を返す
	inline State Excute(int& actionNodeIndex) override
	{
		if (Judge()) return Node::State::Success;
		else return Node::State::Failure;
	}
};

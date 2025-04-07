#pragma once
#include "Node.h"

class Selector : public Node
{
public:
	// 子ノードのindex昇順に見ていき、実行する
	// 子ノードがSuccessを返す -> 自身もSuccessを返す
	// 子ノードがFoundActionを返す -> 自身もFoundActionを返す
	// 子ノードがすべてFailureを返す -> 自身もFailureを返す
	State Excute(int& actionNodeIndex) override;
};


#pragma once
#include "Node.h"

class Sequencer : public Node
{
public:
	// 子ノードのindex昇順に見ていき、実行する
	// 子ノードがFailureを返す -> 自身もFailureを返す
	// 子ノードがFoundActionを返す -> 自身もFoundActionを返す
	// 子ノードがすべてSuccessを返す -> 自身もSuccessを返す
	State Excute(int& actionNodeIndex) override;
};


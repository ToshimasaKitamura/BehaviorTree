#pragma once
#include "Node.h"

class Random : public Node
{
public:
	// 子ノードのランダムに見て、1つだけ実行する
	// 子ノードがSuccessを返す -> 自身もSuccessを返す
	// 子ノードがFoundActionを返す -> 自身もFoundActionを返す
	// 子ノードがすべてFailureを返す -> 自身もFailureを返す
	State Excute(int& actionNodeIndex) override;
};


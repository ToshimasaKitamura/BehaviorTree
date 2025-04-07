#include "Random.h"

Node::State Random::Excute(int& actionNodeIndex)
{
	int index = gsRand(0, GetChildren().size() - 1);
	auto state = GetChildren()[index]->Excute(actionNodeIndex);
	return state;
}

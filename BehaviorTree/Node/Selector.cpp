#include "Selector.h"

Node::State Selector::Excute(int& actionNodeIndex)
{
	for (const auto& child : GetChildren())
	{
		auto state = child->Excute(actionNodeIndex);
		if (state != State::Failure) return state;
	}
	return State::Failure;
}
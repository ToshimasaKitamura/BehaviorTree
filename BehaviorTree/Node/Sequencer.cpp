#include "Sequencer.h"

Node::State Sequencer::Excute(int& actionNodeIndex)
{
	for (const auto& child : GetChildren())
	{
		auto state = child->Excute(actionNodeIndex);
		if (state != State::Success) return state;
	}
	return State::Success;
}
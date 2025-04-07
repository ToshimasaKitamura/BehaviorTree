#include "Root.h"

Node::State Root::Excute(int& actionNodeIndex)
{
    for (const auto& child : GetChildren())
    {
        auto state = child->Excute(actionNodeIndex);
        // FoundActionを返す
        if (state == State::FoundAction) return State::FoundAction;
    }
}

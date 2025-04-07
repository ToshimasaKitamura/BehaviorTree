#include "Node.h"

Node::Common Node::m_common;

void Node::AssignIndex(int index)
{
	m_index = index;
}

void Node::SetParent(const std::shared_ptr<Node> parent)
{
	m_parent = parent;
}

void Node::AddChild(std::shared_ptr<Node> childNode)
{
	childNode->SetParent(shared_from_this());
	m_children.push_back(childNode);
}

void Node::AddChildren(std::vector<std::shared_ptr<Node>> childrenNodes)
{
	for (const auto& child : childrenNodes)
	{
		m_children.push_back(child);
	}
}

void Node::RegisterCommonData(const Common& common)
{
	m_common = common;
}
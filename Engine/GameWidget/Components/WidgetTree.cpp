#include "WidgetTree.h"

WidgetTree::WidgetTree()
{
}

WidgetTree::~WidgetTree()
{
}

void WidgetTree::PlaceWidgetOn(GameWidget* widget, GameWidget* parentWidget)
{
	if (TreeNode* parentNode = FindNode(parentWidget))
	{
		TreeNode node;
		node.Widget = widget;
		parentNode->Children.push_back(node);
	}
	else if (!RootNode.Widget.Get())  // Only Canvas is allowed to be the root node.
	{
		RootNode.Widget = widget;
	}
}

TreeNode* WidgetTree::FindNode(GameWidget* widget)
{
	return FindNode(&RootNode, widget);
}

TreeNode* WidgetTree::FindNode(TreeNode* nodeStart, GameWidget* widget)
{
	if (!nodeStart) return nullptr;
	if (nodeStart->Widget.Get() == widget) return nodeStart;

	for (int i = 0; i < nodeStart->Children.size(); i++)
	{
		TreeNode* node = FindNode(&nodeStart->Children[i], widget);
		if (node != nullptr) return node;
	}

	return nullptr;
}

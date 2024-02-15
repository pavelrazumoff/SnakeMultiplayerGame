#include "WidgetTree.h"

#include "../Utility/WidgetContainerComponent.h"

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
		node.Parent = parentNode;
		parentNode->Children.push_back(node);

		if (WidgetContainerComponent* parentContainerComponent =
			GameWidget::FindWidgetComponent<WidgetContainerComponent>(parentWidget))
		{
			parentContainerComponent->PutWidgetInContainer(widget);
		}
	}
	else if (!RootNode.Widget.Get())  // Only Canvas is allowed to be the root node.
	{
		RootNode.Widget = widget;
	}
}

bool WidgetTree::TryRemoveWidget(GameWidget* widget)
{
	TreeNode* node = FindNode(widget);
	TreeNode* parentNode = node ? node->Parent : nullptr;

	if (!parentNode) return false;
	
	for (int i = 0; i < parentNode->Children.size(); i++)
	{
		if (parentNode->Children[i].Widget.Get() == widget)
		{
			parentNode->Children.erase(parentNode->Children.begin() + i);
			break;
		}
	}

	if (WidgetContainerComponent* parentContainerComponent =
		GameWidget::FindWidgetComponent<WidgetContainerComponent>(parentNode->GetWidget()))
	{
		parentContainerComponent->RemoveWidgetFromContainer(widget);
	}

	return true;
}

TreeNode* WidgetTree::FindNode(GameWidget* widget)
{
	return FindNode(&RootNode, widget);
}

TreeNode* WidgetTree::FindNode(TreeNode* nodeStart, GameWidget* widget)
{
	if (!nodeStart) return nullptr;
	if (widget &&
		(nodeStart->Widget.Get() == widget)) return nodeStart;

	for (int i = 0; i < nodeStart->Children.size(); i++)
	{
		TreeNode* node = FindNode(&nodeStart->Children[i], widget);
		if (node != nullptr) return node;
	}

	return nullptr;
}

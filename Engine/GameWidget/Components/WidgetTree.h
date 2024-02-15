#pragma once

#include "GameWidget.h"
#include <vector>

class TreeNode
{
public:
	TreeNode() {}
	virtual ~TreeNode() {}

	GameWidget* GetWidget() { return Widget.Get(); }
	TreeNode* GetChild(size_t index) { return &Children[index]; }

	size_t GetChildrenCount() const { return Children.size(); }

private:
	friend class WidgetTree;

	TObjectPtr<GameWidget> Widget;

	TreeNode* Parent = nullptr;
	std::vector<TreeNode> Children;
};

class WidgetTree
{
public:
	WidgetTree();
	virtual ~WidgetTree();

	void PlaceWidgetOn(GameWidget* widget, GameWidget* parentWidget);
	bool TryRemoveWidget(GameWidget* widget);

	TreeNode* GetRootNode() { return &RootNode; }

protected:
	TreeNode* FindNode(GameWidget* widget);
	TreeNode* FindNode(TreeNode* nodeStart, GameWidget* widget);

private:
	TreeNode RootNode;
};

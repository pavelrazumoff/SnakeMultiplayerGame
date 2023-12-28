#include "UserWidget.h"
#include "Engine/GameObject/GameObjectUtility.h"

UserWidget::UserWidget()
{
	// TODO: Replace with CreateWidget.
	Canvas = CreateNewObject<CanvasWidget>(this);

	Tree.PlaceWidgetOn(Canvas.Get(), nullptr);
}

UserWidget::~UserWidget()
{
}

void UserWidget::Render()
{
	DrawWidget();
}

void UserWidget::DrawWidget()
{
	DrawWidgetTreeRecursive(Tree.GetRootNode());
}

void UserWidget::ReconstructWidgetTree()
{
	/*
		1. Go down to the deepest widget.
		2. Return his own size to the parent:
			- If this widget has no size (meaning it depends on a parent), return 0 for each dimension with no size.
			- Else, return actual widget size.
		3. Parent should collect all sizes from his children.
			- If there're float-sized widgets, calculate the sum of the rest const sized widgets.
			- Mark this widget as waiting for recalculation after we get the actual size of it's parent.
			- Return either the sum of all children sizes or the sum that we managed to calculate among const sized widgets.
		4. Repeat for all widgets.
		5. After we reach the root widget, we should go reverse and calculate the widgets, that were marked as waiting for recalculation.
		6. For example, if the widget, that is laying under the root const sized widget has a float alignment on the X axis, and this widget
			has 3 children with 2 of them being const sized and 1 float sized (so we have now his size temp calculated as sum only of 2),
			then now we have it's parent size that this widget should be filled in. So we can get the size of the 3rd child
			by subtracting the temp sum from the parent size.
	*/

	CalculateDirtySizeWidgetsRecursive(Tree.GetRootNode());

	// TODO: Get UserWidget position on screen and pass here.
	RC_RECT canvasRect = { 0, 0, (RC_UINT)Canvas->GetWidth(), (RC_UINT)Canvas->GetHeight() };
	Canvas->RepositionWidget(canvasRect);

	ClarifyUnderlaySizeWidgetsRecursive(Tree.GetRootNode());
}

void UserWidget::CalculateDirtySizeWidgetsRecursive(TreeNode* node)
{
	if (!node) return;

	const size_t numChildren = node->GetChildrenCount();
	for (size_t i = 0; i < numChildren; i++)
	{
		TreeNode* childNode = node->GetChild(i);
		CalculateDirtySizeWidgetsRecursive(childNode);
	}

	if (GameWidget* widget = node->GetWidget())
	{
		widget->ReconstructWidget();
	}
}

void UserWidget::ClarifyUnderlaySizeWidgetsRecursive(TreeNode* node)
{
	if (!node) return;

	/*
		We should take the node and treat it as handled widget.
		Now we need to take its actual rect and go through each of the children and call specific implementation to calculate actual
		rect of child widget based on the parent widget layouting (eg. vertical/horizontal box) and layouting of the child itself.
	*/

	const size_t numChildren = node->GetChildrenCount();
	GameWidget** underlayWidgets = new GameWidget * [numChildren];

	for (size_t i = 0; i < numChildren; i++)
	{
		TreeNode* childNode = node->GetChild(i);
		underlayWidgets[i] = childNode ? childNode->GetWidget() : nullptr;
	}

	if (GameWidget* widget = node->GetWidget())
	{
		widget->ReconstructUnderlayWidgets(underlayWidgets, numChildren);
	}

	delete[] underlayWidgets;
	underlayWidgets = nullptr;
	
	for (size_t i = 0; i < numChildren; i++)
	{
		TreeNode* childNode = node->GetChild(i);
		ClarifyUnderlaySizeWidgetsRecursive(childNode);
	}
}

void UserWidget::DrawWidgetTreeRecursive(TreeNode* node)
{
	if (GameWidget* widget = node->GetWidget())
	{
		widget->DrawWidget();
	}

	const size_t numChildren = node->GetChildrenCount();
	for (size_t i = 0; i < numChildren; i++)
	{
		TreeNode* childNode = node->GetChild(i);
		DrawWidgetTreeRecursive(childNode);
	}
}

void UserWidget::SetCanvasDimensions(uint32_t width, uint32_t height)
{
	Canvas->SetWidth(width);
	Canvas->SetHeight(height);

	ReconstructWidgetTree();
}

void UserWidget::GetCanvasDimensions(uint32_t& width, uint32_t& height) const
{
	width = Canvas->GetWidth();
	height = Canvas->GetHeight();
}

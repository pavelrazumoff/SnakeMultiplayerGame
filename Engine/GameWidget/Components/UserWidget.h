#pragma once

#include "CanvasWidget.h"
#include "WidgetTree.h"

#include "Engine/Render/Renderable.h"

class UserWidget : public GameWidget, public IRenderable
{
public:
	UserWidget();
	virtual ~UserWidget();

	/** Extending the IRenderable interface. */
	virtual void Render() override;

	virtual void DrawWidget() override;

	void SetCanvasDimensions(uint32_t width, uint32_t height);
	void GetCanvasDimensions(uint32_t& width, uint32_t& height) const;

protected:
	void ReconstructWidgetTree();

private:
	void CalculateDirtySizeWidgetsRecursive(TreeNode* node);
	void ClarifyUnderlaySizeWidgetsRecursive(TreeNode* node);

	void DrawWidgetTreeRecursive(TreeNode* node);

protected:
	WidgetTree Tree;

	TObjectPtr<CanvasWidget> Canvas;
};

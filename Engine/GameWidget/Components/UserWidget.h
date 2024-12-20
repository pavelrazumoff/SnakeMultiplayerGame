#pragma once

#include "CanvasWidget.h"
#include "WidgetTree.h"

#include "Engine/Render/Renderable.h"
#include "Engine/Input/InputHandler.h"

#include <functional>

class UserWidget : public GameWidget, public IRenderable, public IInputHandler
{
public:
	UserWidget();
	virtual ~UserWidget();

	/** Extending the IRenderable interface. */

	virtual void Render(RCTexture* RenderTargetTexture) override;
	virtual bool IsVisible() const override { return true; }

	virtual RenderPriorityGroup GetRenderPriorityGroup() const override { return RenderPriorityGroup::UI; }

	/** Extending the IInputHandler interface. */

	virtual bool PassInput(const InputState& is) override;

	/**  */

	virtual void DrawWidget(RCTexture* RenderTargetTexture) override;

	void SetCanvasDimensions(uint32_t width, uint32_t height);
	void GetCanvasDimensions(uint32_t& width, uint32_t& height) const;

	void ForceReconstruct();
	bool RemoveWidgetFromUserWigdet(GameWidget* widget);

	bool DoesBelongToWidgetTree(GameWidget* widget);

	void SetWidgetModality(bool bModal);
	bool IsWidgetModal() const { return bModalWidget; }

	WidgetTree& GetWidgetTree() { return Tree; }

protected:
	void ReconstructWidgetTree();

private:
	void CalculateDirtySizeWidgetsRecursive(TreeNode* node);
	void ClarifyUnderlaySizeWidgetsRecursive(TreeNode* node);

	bool ProceedWidgetTreeRecursive(TreeNode* node, std::function<bool(GameWidget*, bool&)> func);

protected:
	WidgetTree Tree;

	TObjectPtr<CanvasWidget> Canvas;

private:
	bool bModalWidget = false;
};

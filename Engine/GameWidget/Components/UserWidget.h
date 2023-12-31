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

	/** Extending the IInputHandler interface. */
	virtual bool PassInput(const InputState& is) override;

	virtual void DrawWidget(RCTexture* RenderTargetTexture) override;

	void SetCanvasDimensions(uint32_t width, uint32_t height);
	void GetCanvasDimensions(uint32_t& width, uint32_t& height) const;

protected:
	void ReconstructWidgetTree();

private:
	void CalculateDirtySizeWidgetsRecursive(TreeNode* node);
	void ClarifyUnderlaySizeWidgetsRecursive(TreeNode* node);

	bool ProceedWidgetTreeRecursive(TreeNode* node, std::function<bool(GameWidget*)> func);

protected:
	WidgetTree Tree;

	TObjectPtr<CanvasWidget> Canvas;
};

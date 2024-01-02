#pragma once

#include "Engine/GameObject/GameObject.h"
#include "../Utility/BrushStyle.h"

#include "RenderCore/Core/RCTexture.h"

#include <memory>

class BrushPrimitive : public GameObject
{
public:
	BrushPrimitive();
	virtual ~BrushPrimitive();

	virtual void Construct();
	virtual void Draw(RCTexture* RenderTargetTexture);

	void UpdateDrawRect(const TEX_RECT& newRect);
	TEX_RECT GetDrawRect() const { return DrawRect; }

public:
	void SetBrushStyle(const BrushStyle& newStyle) { Style = newStyle; }
	const BrushStyle& GetBrushStyle() const { return Style; }

protected:
	virtual GameObject* Clone(GameObject* _owner) const override;

protected:
	BrushStyle Style;

private:
	std::shared_ptr<RCTexture> BrushTexture;
	TEX_RECT DrawRect;
};

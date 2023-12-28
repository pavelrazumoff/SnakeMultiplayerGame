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
	virtual void Draw();

	void UpdateDrawRect(const TEX_RECT& newRect);

public:
	void SetBrushStyle(const BrushStyle& newStyle) { Style = newStyle; }
	const BrushStyle& GetBrushStyle() const { return Style; }

protected:
	BrushStyle Style;

private:
	std::shared_ptr<RCTexture> BrushTexture;
	TEX_RECT DrawRect;
};

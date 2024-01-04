#pragma once

#include "stdint.h"

class RCTexture;

enum class RenderPriorityGroup
{
	SceneObject,
	UI,

	Other,
};

class IRenderable
{
public:
	virtual void Render(RCTexture* RenderTargetTexture) = 0;
	virtual bool IsVisible() const = 0;

	virtual RenderPriorityGroup GetRenderPriorityGroup() const = 0;
	virtual uint8_t GetRenderPriority() const { return 0; } // 0 is default priority, > 0 is higher priority (draw on top of lowest).
};

#pragma once

class RCTexture;

class IRenderable
{
public:
	virtual void Render(RCTexture* RenderTargetTexture) = 0;
};

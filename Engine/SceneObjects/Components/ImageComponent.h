#pragma once

#include "SceneComponent.h"

#include "Core/RCTexture.h"
#include <memory>

class ImageComponent : public SceneComponent
{
public:
	ImageComponent() {}
	virtual ~ImageComponent() {}

	void SetImage(std::shared_ptr<RCTexture> image);

	virtual void DrawComponent(RCTexture* RenderTargetTexture);

protected:
	std::shared_ptr<RCTexture> Image;
};

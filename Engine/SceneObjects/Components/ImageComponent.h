#pragma once

#include "SceneComponent.h"

#include "Core/RCTexture.h"
#include <memory>

class ImageComponent : public SceneComponent
{
public:
	ImageComponent() {}
	virtual ~ImageComponent() {}

	virtual void DrawComponent(RCTexture* RenderTargetTexture);

	void SetImage(std::shared_ptr<RCTexture> image);
	void SetImageSize(LV_SIZE newSize);

	const RCTexture* GetImage() const { return Image.get(); }
	LV_SIZE GetImageSize() const { return ImageSize; }

protected:
	void Construct();

protected:
	std::shared_ptr<RCTexture> Image;
	LV_SIZE ImageSize = { 0.0f, 0.0f };

private:
	std::shared_ptr<RCTexture> DrawTexture;
};

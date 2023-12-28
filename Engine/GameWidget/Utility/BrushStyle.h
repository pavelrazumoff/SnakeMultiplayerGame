#pragma once

#include "RenderCore/Core/RCTexture.h"
#include <memory>

struct BrushStyle
{
public:
	bool DoesUseImage() const { return (Image.get() != nullptr); }

public:
	TEX_SIZE Dimensions = { 0 };

	std::shared_ptr<RCTexture> Image;
	TEX_PIXEL FillPixel = 0;
};

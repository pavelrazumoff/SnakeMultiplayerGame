#pragma once

#include "RenderCore/Core/RCTexture.h"
#include <memory>

struct BrushStyle
{
public:
	BrushStyle& operator=(const BrushStyle& other)
	{
		if (this == &other) return *this;

		Dimensions = other.Dimensions;

		if (other.Image.get())
			Image.reset(other.Image->CopyTexture());

		FillPixel = other.FillPixel;
		FillColorRGB = other.FillColorRGB;

		return *this;
	}

	bool DoesUseImage() const { return (Image.get() != nullptr); }

public:
	TEX_SIZE Dimensions = { 0 };

	std::shared_ptr<RCTexture> Image;
	TEX_PIXEL FillPixel = 0;
	TEX_COLOR_RGB FillColorRGB = RenderConstants::WhitePixelColorRGB;
};

#include "BrushPrimitive.h"

#include "RenderCore/Core/RenderTextureLibrary.h"
#include "Engine/GameObject/GameObjectUtility.h"

BrushPrimitive::BrushPrimitive()
{
	DrawRect = { 0, 0, 0, 0 };
}

BrushPrimitive::~BrushPrimitive()
{
}

void BrushPrimitive::Construct()
{
	RC_UINT texWidth = (Style.Dimensions.cx > 0) ? Style.Dimensions.cx : GetTexWidth(DrawRect);
	RC_UINT texHeight = (Style.Dimensions.cy > 0) ? Style.Dimensions.cy : GetTexHeight(DrawRect);

	BrushTexture = std::make_shared<RCTexture>(texWidth, texHeight);
	if (Style.DoesUseImage())
	{
		TEX_COORD coord = { 0, 0 };

		std::shared_ptr<RCTexture> drawImg = Style.Image;
		if (texWidth != Style.Image->GetWidth() ||
			texHeight != Style.Image->GetHeight())
		{
			TEX_SIZE newSize = { texWidth, texHeight };
			drawImg = std::shared_ptr<RCTexture>(RenderTextureLibrary::StretchTexture(Style.Image.get(), newSize));
		}

		RenderTextureLibrary::FillTexture(BrushTexture.get(), drawImg.get(), coord);
	}
	else
	{
		RenderTextureLibrary::FillTexture(BrushTexture.get(), Style.FillPixel);
		RenderTextureLibrary::FillTextureColor(BrushTexture.get(), Style.FillColorRGB);
	}
}

void BrushPrimitive::Draw(RCTexture* RenderTargetTexture)
{
	TEX_COORD coord = { DrawRect.left, DrawRect.top };
	RenderTextureLibrary::FillTexture(RenderTargetTexture, BrushTexture.get(), coord);
}

void BrushPrimitive::UpdateDrawRect(const TEX_RECT& newRect)
{
	DrawRect = newRect;
}

GameObject* BrushPrimitive::Clone(GameObject* _owner) const
{
	BrushPrimitive* newBrush = CreateNewObject<BrushPrimitive>(_owner);
	newBrush->SetBrushStyle(Style);

	return newBrush;
}

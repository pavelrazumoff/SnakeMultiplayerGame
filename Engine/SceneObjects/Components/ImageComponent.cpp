#include "ImageComponent.h"

#include "RenderCore/Core/RenderTextureLibrary.h"

void ImageComponent::DrawComponent(RCTexture* RenderTargetTexture)
{
	// TODO: Move to utility.
	const RC_UINT texWidth = (RC_UINT)floor(ImageSize.x);
	const RC_UINT texHeight = (RC_UINT)floor(ImageSize.y);

	const LV_COORD drawLoc = GetSceneLocation();
	TEX_RECT VisibleRect = { 0, 0, texWidth - 1, texHeight - 1 };
	TEX_COORD DrawCoord = { 0, 0 };

	if (drawLoc.x < 0.0f)
		VisibleRect.left = (RC_INT)round(-drawLoc.x);
	else
		DrawCoord.x = (RC_INT)round(drawLoc.x);

	if (drawLoc.y < 0.0f)
		VisibleRect.top = (RC_INT)round(-drawLoc.y);
	else
		DrawCoord.y = (RC_INT)round(drawLoc.y);

	if (VisibleRect.left > VisibleRect.right ||
		VisibleRect.top > VisibleRect.bottom) return;

	if (VisibleRect.left >= RenderTargetTexture->GetWidth() ||
		VisibleRect.top >= RenderTargetTexture->GetHeight()) return;

	RenderTextureLibrary::FillTexture(RenderTargetTexture, DrawTexture.get(), VisibleRect, DrawCoord);
}

void ImageComponent::SetImage(std::shared_ptr<RCTexture> image)
{
	Image = image;
}

void ImageComponent::SetImageSize(LV_SIZE newSize)
{
	ImageSize = newSize;
	Construct();
}

void ImageComponent::Construct()
{
	RC_UINT texWidth = (RC_UINT)floor(ImageSize.x);
	RC_UINT texHeight = (RC_UINT)floor(ImageSize.y);

	DrawTexture = std::make_shared<RCTexture>(texWidth, texHeight);

	TEX_COORD coord = { 0, 0 };

	std::shared_ptr<RCTexture> drawImg = Image;
	if (texWidth != Image->GetWidth() ||
		texHeight != Image->GetHeight())
	{
		TEX_SIZE newSize = { texWidth, texHeight };
		drawImg = std::shared_ptr<RCTexture>(RenderTextureLibrary::StretchTexture(Image.get(), newSize));
	}

	RenderTextureLibrary::FillTexture(DrawTexture.get(), drawImg.get(), coord);
}

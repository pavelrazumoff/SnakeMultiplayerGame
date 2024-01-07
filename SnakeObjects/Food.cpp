#include "Food.h"

#include "Engine/SceneObjects/Components/ImageComponent.h"
#include "Engine/SceneObjects/Components/BoxComponent.h"

#include "Core/RenderTextureLibrary.h"

FoodObject::FoodObject()
{
	std::shared_ptr<RCTexture> imgFood = std::make_shared<RCTexture>(1, 1);
	RenderTextureLibrary::FillTexture(imgFood.get(), 'f');
	RenderTextureLibrary::FillTextureColor(imgFood.get(), RenderConstants::YellowPixelColorRGB);

	// Image.
	{
		FoodImageComponent = CreateChildComponent<ImageComponent>();

		FoodImageComponent->SetImage(imgFood);
		FoodImageComponent->SetImageSize({ 1.0f, 1.0f });
	}

	// Collision Box.
	{
		FoodBoxComponent = CreateChildComponent<BoxComponent>();
		FoodBoxComponent->SetBoxExtent({ 0.5f, 0.5f });
	}
}

FoodObject::~FoodObject()
{
}

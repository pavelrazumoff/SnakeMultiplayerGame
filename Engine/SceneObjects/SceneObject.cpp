#include "SceneObject.h"

#include "Components/SceneComponent.h"

SceneObject::SceneObject()
{
}

SceneObject::~SceneObject()
{
}

void SceneObject::SetLocation(const LV_COORD& newLocation)
{
	Location = newLocation;
}

void SceneObject::Render(RCTexture* RenderTargetTexture)
{
	/** We must strive to do all render operations with the help of child components, not the object itself. */

	for (auto& child : ChildComponents)
	{
		if (auto sceneComp = dynamic_cast<SceneComponent*>(child))
			sceneComp->DrawComponent(RenderTargetTexture);
	}
}

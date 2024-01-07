#include "SceneComponent.h"

void SceneComponent::UpdateComponent(float /*DeltaTime*/)
{

}

void SceneComponent::UpdateSceneLocation(LV_COORD parentLocation)
{
	if (bUseAbsoulteLocation) return;

	SetSceneLocation(parentLocation + RelativeLocation);
}

void SceneComponent::SetRelativeLocation(LV_COORD relLocation)
{
	RelativeLocation = relLocation;
}

void SceneComponent::SetAbsoluteLocation(LV_COORD sceneLocation)
{
	// Note: If bUseAbsoulteLocation is false then
	// this will override the relative location based on the parent, but will be reset with the next UpdateSceneLocation call.
	SetSceneLocation(sceneLocation);
}

void SceneComponent::SetSceneLocation(LV_COORD sceneLocation)
{
	SceneLocation = sceneLocation;
}

#include "SceneComponent.h"

void SceneComponent::UpdateComponent(float /*DeltaTime*/)
{

}

void SceneComponent::UpdateSceneLocation(LV_COORD parentLocation)
{
	SceneLocation = parentLocation + RelativeLocation;
}

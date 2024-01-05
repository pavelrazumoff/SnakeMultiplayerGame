#include "SceneObject.h"

#include "Components/SceneComponent.h"
#include "Components/InputComponent.h"

SceneObject::SceneObject()
{
}

SceneObject::~SceneObject()
{
}

void SceneObject::BeginPlay()
{
	for (auto& child : ChildComponents)
	{
		child->BeginPlayComponent();
	}
}

void SceneObject::Update(float DeltaTime)
{
	for (auto& child : ChildComponents)
	{
		if (auto sceneComp = dynamic_cast<SceneComponent*>(child))
			sceneComp->UpdateSceneLocation(Location);

		child->UpdateComponent(DeltaTime);
	}
}

void SceneObject::SetLocation(const LV_COORD& newLocation)
{
	Location = newLocation;

	for (auto& child : ChildComponents)
	{
		if (auto sceneComp = dynamic_cast<SceneComponent*>(child))
			sceneComp->UpdateSceneLocation(Location);
	}
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

bool SceneObject::PassInput(const InputState& is)
{
	/** Only InputComponent (or his heirs) is allowed to handle the raw input state. */
	if (auto pInputComp = SceneObject::FindComponent<InputComponent>(this))
		return pInputComp->PassInput(is);
	return false;
}

void SceneObject::AddObjectComponent(ObjectComponent* newComponent)
{
	ChildComponents.push_back(newComponent);
}

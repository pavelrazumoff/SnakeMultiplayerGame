#include "SceneObject.h"

#include "Components/SceneComponent.h"
#include "Components/InputComponent.h"

#include "Engine/Level/LevelManager.h"
#include "Engine/Network/ReplicationUtility.h"

SceneObject::SceneObject()
{
}

SceneObject::~SceneObject()
{
}

void SceneObject::RegisterReplicationMembers()
{
	MAKE_REPLICATED(SceneObject, Location, EPrimitiveType::EPT_Vector2D, nullptr);
	MAKE_REPLICATED_ARRAY(SceneObject, ChildComponentsNetIDs, EPrimitiveType::EPT_Array, EPrimitiveType::EPT_Int, nullptr);
}

void SceneObject::PostReplCreate()
{
	Inherited::PostReplCreate();

	if (NetworkUtility::IsClient())
	{
		uint32_t numComponentIds = ChildComponentsNetIDs.Size();
		engine_assert(numComponentIds == ChildComponentsReplicated.size());

		for (uint32_t i = 0; i < numComponentIds; ++i)
		{
			uint32_t componentNetId = ChildComponentsNetIDs[i];
			auto childComponent = ChildComponentsReplicated[i].Get();

			ReplicationManager::GetInstance().AssignNetworkIdForReplicationObject(childComponent, componentNetId);
		}
	}

	LevelManager::GetInstance().PlaceObjectOnLevel(this, Location);
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
		if (auto sceneComp = dynamic_cast<SceneComponent*>(child.Get()))
			sceneComp->UpdateSceneLocation(Location);

		child->UpdateComponent(DeltaTime);
	}
}

void SceneObject::SetLocation(const LV_COORD& newLocation)
{
	if (Location == newLocation) return;

	Location = newLocation;
	MARK_FOR_REPLICATION(SceneObject, Location);

	for (auto& child : ChildComponents)
	{
		if (auto sceneComp = dynamic_cast<SceneComponent*>(child.Get()))
			sceneComp->UpdateSceneLocation(Location);
	}
}

void SceneObject::Render(RCTexture* RenderTargetTexture)
{
	/** We must strive to do all render operations with the help of child components, not the object itself. */

	for (auto& child : ChildComponents)
	{
		if (auto sceneComp = dynamic_cast<SceneComponent*>(child.Get()))
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

	if (newComponent->IsReplicationEnabled())
	{
		if (NetworkUtility::IsServer())
		{
			ChildComponentsNetIDs.Add(
				ReplicationManager::GetInstance().GetNetworkIdForObject(newComponent));
		}

		ChildComponentsReplicated.push_back(newComponent);
	}
}

void SceneObject::RemoveObjectComponent(ObjectComponent* componentToRemove)
{
	auto it = std::find_if(ChildComponents.begin(), ChildComponents.end(), [componentToRemove](auto& Other) -> bool {
		return componentToRemove == Other.Get();
		});

	if (it != ChildComponents.end()) ChildComponents.erase(it);
}

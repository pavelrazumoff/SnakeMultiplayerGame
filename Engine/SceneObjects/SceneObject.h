#pragma once

#include "Engine/GameObject/GameObject.h"

#include "Engine/Input/InputHandler.h"
#include "Engine/EngineTypes/Array.h"

#include "Engine/Render/Renderable.h"
#include "Components/ObjectComponent.h"

#include "Engine/GameObject/GameObjectPtr.h"
#include "Engine/GameObject/GameObjectUtility.h"

REGISTER_CLASS(SceneObject)
REGISTER_CLASS_FOR_REPLICATION(SceneObject)
class SceneObject : public GameObject, public IInputHandler, public IRenderable
{
	GAMEOBJECT_BODY(SceneObject, GameObject)
	ENABLE_REPLICATION(SceneObject)

public:
	SceneObject();
	virtual ~SceneObject();

	virtual void BeginPlay();
	virtual void Update(float DeltaTime);

	/** Scene Placement. */

	virtual void SetLocation(const LV_COORD& newLocation);
	const LV_COORD& GetLocation() const { return Location; }

	/** Extending the IRenderable interface. */

	virtual void Render(RCTexture* RenderTargetTexture) override;
	virtual bool IsVisible() const override { return true; }

	virtual RenderPriorityGroup GetRenderPriorityGroup() const override { return RenderPriorityGroup::SceneObject; }

	/** Extending the IInputHandler interface. */

	virtual bool PassInput(const InputState& is) override final;

	/** */

	template <class T>
	T* CreateChildComponent()
	{
		T* newComponent = CreateNewObject<T>(this);
		AddObjectComponent(newComponent);
		return newComponent;
	}

	void AddObjectComponent(ObjectComponent* newComponent);
	void RemoveObjectComponent(ObjectComponent* componentToRemove);

	template <class T>
	static T* FindComponent(SceneObject* object)
	{
		for (auto& component : object->ChildComponents)
		{
			if (T* castedComponent = dynamic_cast<T*>(component.Get()))
				return castedComponent;
		}

		return nullptr;
	}

protected:
	/** IReplicationObject implementation. */

	virtual void PostReplCreate() override;

	void OnReplicate_SetLocationCallback();

protected:
	LV_COORD Location = { 0, 0 };

private:
	std::vector<TObjectPtr<ObjectComponent>> ChildComponents;
	std::vector<TObjectPtr<ObjectComponent>> ChildComponentsReplicated;

	TArray<uint32_t> ChildComponentsNetIDs;
};

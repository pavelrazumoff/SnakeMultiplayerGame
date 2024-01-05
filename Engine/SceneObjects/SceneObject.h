#pragma once

#include "Engine/GameObject/GameObject.h"

#include "Engine/Input/InputHandler.h"
#include "Engine/EngineTypes.h"

#include "Engine/Render/Renderable.h"
#include "Components/ObjectComponent.h"

class SceneObject : public GameObject, public IInputHandler, public IRenderable
{
	typedef GameObject Inherited;

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

	void AddObjectComponent(ObjectComponent* newComponent);
	void RemoveObjectComponent(ObjectComponent* componentToRemove);

	template <class T>
	static T* FindComponent(SceneObject* object)
	{
		for (auto& component : object->ChildComponents)
		{
			if (T* castedComponent = dynamic_cast<T*>(component))
				return castedComponent;
		}

		return nullptr;
	}

protected:
	LV_COORD Location = { 0, 0 };

private:
	std::vector<ObjectComponent*> ChildComponents;
};

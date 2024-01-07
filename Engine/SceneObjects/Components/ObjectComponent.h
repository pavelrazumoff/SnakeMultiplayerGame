#pragma once

#include "Engine/GameObject/GameObject.h"

class SceneObject;

class ObjectComponent : public GameObject
{
public:
	virtual ~ObjectComponent() {}

	virtual void BeginPlayComponent() {}
	virtual void UpdateComponent(float /*DeltaTime*/) {}
};

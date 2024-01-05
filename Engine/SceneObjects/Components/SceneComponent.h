#pragma once

#include "ObjectComponent.h"
#include "Engine/EngineTypes.h"

class RCTexture;

class SceneComponent : public ObjectComponent
{
public:
	virtual ~SceneComponent() {}

	virtual void DrawComponent(RCTexture* /*RenderTargetTexture*/) {}
	virtual void UpdateComponent(float DeltaTime);

	void UpdateSceneLocation(LV_COORD parentLocation);

	LV_COORD GetSceneLocation() const { return SceneLocation; }

protected:
	LV_COORD RelativeLocation = { 0, 0 };

private:
	LV_COORD SceneLocation = { 0, 0 };
};

#pragma once

#include "ObjectComponent.h"
#include "Engine/EngineTypes.h"

class RCTexture;

class SceneComponent : public ObjectComponent
{
public:
	virtual ~SceneComponent() {}

	virtual void DrawComponent(RCTexture* /*RenderTargetTexture*/) {}

protected:
	LV_COORD RelativeLocation = { 0, 0 };

private:
	LV_COORD SceneLocation = { 0, 0 };
};

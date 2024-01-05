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
	void SetAbsoluteLocation(LV_COORD sceneLocation);

	void SetUseAbsoluteLocation(bool bAbsoluteLoc) { bUseAbsoulteLocation = bAbsoluteLoc; }

	LV_COORD GetSceneLocation() const { return SceneLocation; }

protected:
	LV_COORD RelativeLocation = { 0, 0 };
	bool bUseAbsoulteLocation = false;

private:
	LV_COORD SceneLocation = { 0, 0 };
};

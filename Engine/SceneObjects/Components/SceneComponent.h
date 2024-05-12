#pragma once

#include "ObjectComponent.h"
#include "Engine/EngineTypes/EngineBasicTypes.h"

class RCTexture;

REGISTER_CLASS(SceneComponent)
REGISTER_CLASS_FOR_REPLICATION(SceneComponent)
class SceneComponent : public ObjectComponent
{
	GAMEOBJECT_BODY(SceneComponent, ObjectComponent)
	ENABLE_REPLICATION(SceneComponent)

public:
	virtual ~SceneComponent() {}

	virtual void DrawComponent(RCTexture* /*RenderTargetTexture*/) {}

	void UpdateSceneLocation(LV_COORD parentLocation);

	void SetRelativeLocation(LV_COORD relLocation);
	void SetAbsoluteLocation(LV_COORD sceneLocation);

	void SetUseAbsoluteLocation(bool bAbsoluteLoc) { bUseAbsoulteLocation = bAbsoluteLoc; }

	LV_COORD GetSceneLocation() const { return SceneLocation; }
	LV_COORD GetRelativeLocation() const { return RelativeLocation; }

protected:
	virtual void SetSceneLocation(LV_COORD sceneLocation);

protected:
	LV_COORD RelativeLocation = { 0, 0 };
	bool bUseAbsoulteLocation = false;

private:
	LV_COORD SceneLocation = { 0, 0 };
};

#pragma once

#include "Engine/SceneObjects/SceneObject.h"

class ImageComponent;
class BoxComponent;

REGISTER_CLASS(FoodObject)
class FoodObject : public SceneObject
{
	GAMEOBJECT_BODY(FoodObject, SceneObject)

public:
	FoodObject();
	virtual ~FoodObject();

protected:
	TObjectPtr<ImageComponent> FoodImageComponent;
	TObjectPtr<BoxComponent> FoodBoxComponent;
};

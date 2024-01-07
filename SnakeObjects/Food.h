#pragma once

#include "Engine/SceneObjects/SceneObject.h"

class ImageComponent;
class BoxComponent;

class FoodObject : public SceneObject
{
	typedef SceneObject Inherited;

public:
	FoodObject();
	virtual ~FoodObject();

protected:
	TObjectPtr<ImageComponent> FoodImageComponent;
	TObjectPtr<BoxComponent> FoodBoxComponent;
};

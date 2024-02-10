#pragma once

#include "WidgetComponent.h"
#include "Core/RCDataTypes.h"

#include "Engine/GameObject/GameObjectPtr.h"

#include <vector>

class GameWidget;

class WidgetContainerComponent : public IWidgetComponent
{
public:
	void PutWidgetInContainer(GameWidget* widget);
	uint32_t GetNumWidgetsInContainer() const;
	TObjectPtr<GameWidget> GetWidgetFromContainer(int index);

	void ResetContainerFreeRect(RC_RECT rect);
	void ResetAlignment(bool bVertical);

	void PlaceInContainer(const RC_RECT& occupiedRect);
	RC_RECT GetContainerFreeRect() const { return ContainerFreeRect; }

protected:
	std::vector<TObjectPtr<GameWidget>> WidgetsInContainer;

private:
	RC_RECT ContainerFreeRect = { 0 };
	bool bVerticalAlignment = true;
};

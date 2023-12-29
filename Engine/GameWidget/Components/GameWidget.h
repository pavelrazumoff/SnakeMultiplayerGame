#pragma once

#include "Engine/GameObject/GameObject.h"
#include "Engine/GameObject/GameObjectPtr.h"
#include "Core/RCDataTypes.h"
#include "../Utility/WidgetComponent.h"

class RCTexture;

class GameWidget : public GameObject
{
public:
	GameWidget();
	virtual ~GameWidget();

	virtual void ReconstructWidget();
	virtual void ReconstructUnderlayWidgets(GameWidget** underlayWidgets, size_t underlayWidgetsCount);
	virtual void RepositionWidget(const RC_RECT& newRect);

	virtual void DrawWidget(RCTexture* RenderTargetTexture) {}

	RC_SIZE GetCachedDirtySize() const { return CachedDirtySize; }
	bool IsSizeXNeedsToRecalc() const { return bSizeXNeedsToRecalc; }
	bool IsSizeYNeedToRecalc() const { return bSizeYNeedToRecalc; }

	RC_RECT GetCachedActualRect() const { return CachedActualRect; }

	template<typename T>
	static T* FindWidgetComponent(GameWidget* widget)
	{
		if (!widget) return nullptr;

		for (IWidgetComponent* component : widget->WidgetComponents)
		{
			if (T* castedComponent = dynamic_cast<T*>(component))
			{
				return castedComponent;
			}
		}

		return nullptr;
	}

protected:
	virtual RC_SIZE CalcDirtySize(bool& _bSizeXNeedsToRecalc, bool& _bSizeYNeedToRecalc);

	void AddWidgetComponent(IWidgetComponent* newComponent);

protected:
	std::vector<IWidgetComponent*> WidgetComponents;

private:
	RC_SIZE CachedDirtySize = { 0, 0 };
	bool bSizeXNeedsToRecalc = false;
	bool bSizeYNeedToRecalc = false;

	RC_RECT CachedActualRect = { 0, 0, 0, 0 };
};

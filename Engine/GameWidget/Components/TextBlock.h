#pragma once

#include "GameWidget.h"

#include "../Utility/AlignmentSettings.h"
#include "../Utility/LayoutSettings.h"
#include "../Primitives/TextPrimitive.h"

class TextBlock : public GameWidget
{
	typedef GameWidget Inheried;

public:
	TextBlock();
	virtual ~TextBlock();

	virtual void RepositionWidget(const RC_RECT& newRect) override;
	virtual void DrawWidget(RCTexture* RenderTargetTexture) override;

	virtual RC_SIZE CalcDirtySize(bool& _bSizeXNeedsToRecalc, bool& _bSizeYNeedToRecalc) override;

	AlignmentSettings& GetAlignment() { return Alignment; }
	LayoutSettings& GetLayout() { return Layout; }

	TextPrimitive& GetText() { return *Text.Get(); }

protected:
	virtual GameObject* Clone(GameObject* _owner) const override;

protected:
	AlignmentSettings Alignment;
	LayoutSettings Layout;

	TObjectPtr<TextPrimitive> Text;
};

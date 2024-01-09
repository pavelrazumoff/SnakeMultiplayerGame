#pragma once

#include "Engine/GameWidget/Components/UserWidget.h"

class VerticalBox;
class TextBlock;

class PlayerHUDWidget : public UserWidget
{
	typedef UserWidget Inherited;

public:
	PlayerHUDWidget();
	~PlayerHUDWidget();

	void SetScore(uint32_t score);

	RC_RECT GetScreenFreeRect() const;

protected:
	TObjectPtr<VerticalBox> WndVerticalBox;

	TObjectPtr<TextBlock> ScoreText;
};

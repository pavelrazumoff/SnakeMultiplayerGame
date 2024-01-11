#include "PlayerHUDWidget.h"

#include "Engine/GameWidget/Components/VerticalBox.h"
#include "Engine/GameWidget/Components/TextBlock.h"

#include "Engine/GameObject/GameObjectUtility.h"

PlayerHUDWidget::PlayerHUDWidget()
{
	WndVerticalBox = CreateNewObject<VerticalBox>(this);
	if (WndVerticalBox.Get())
	{
		Tree.PlaceWidgetOn(WndVerticalBox.Get(), Canvas.Get());
	}

	ScoreText = CreateNewObject<TextBlock>(this);
	if (ScoreText.Get())
	{
		ScoreText->GetAlignment().Horizontal = AlignmentSettings::HorizontalAlignment::Left;
		ScoreText->GetAlignment().Vertical = AlignmentSettings::VerticalAlignment::Top;
		ScoreText->GetAlignment().Stretch = AlignmentSettings::StretchMode::NoStretch;
		ScoreText->GetAlignment().Padding = { 2, 1, 0, 0 };

		ScoreText->GetText().SetText("Score: 0");
		ScoreText->GetText().SetFontStyle({ 1, FontPrintType::Simple, RenderConstants::LightOrangePixelColorRGB });

		Tree.PlaceWidgetOn(ScoreText.Get(), WndVerticalBox.Get());
	}
}

PlayerHUDWidget::~PlayerHUDWidget()
{
}

void PlayerHUDWidget::SetScore(uint32_t score)
{
	if (ScoreText.IsValid())
	{
		ScoreText->SetText("Score: " + std::to_string(score));
	}
}

RC_RECT PlayerHUDWidget::GetScreenFreeRect() const
{
	if (!ScoreText.IsValid())
		return RC_RECT();

	RC_RECT freeRect = Canvas->GetCachedActualRect();
	RC_RECT scoreRect = ScoreText->GetCachedActualRect();

	return { freeRect.left, RC_UINT(scoreRect.bottom + 1), freeRect.right, freeRect.bottom };
}

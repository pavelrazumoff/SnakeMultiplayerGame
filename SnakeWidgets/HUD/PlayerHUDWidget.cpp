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

		ScoreText->GetText().SetText("Score: 0");
		ScoreText->GetText().SetFontStyle({ 1, FontPrintType::Letter, RenderConstants::LightOrangePixelColorRGB });

		Tree.PlaceWidgetOn(ScoreText.Get(), WndVerticalBox.Get());
	}
}

PlayerHUDWidget::~PlayerHUDWidget()
{
}

void PlayerHUDWidget::SetScore(uint32_t score)
{
	if (ScoreText.Get())
	{
		ScoreText->SetText("Score: " + std::to_string(score));
	}
}

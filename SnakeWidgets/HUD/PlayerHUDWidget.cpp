#include "PlayerHUDWidget.h"

#include "Engine/GameWidget/Components/HorizontalBox.h"
#include "Engine/GameWidget/Components/TextBlock.h"
#include "Engine/GameWidget/Components/PanelWidget.h"
#include "Engine/GameWidget/Components/Button.h"

#include "Engine/GameObject/GameObjectUtility.h"

PlayerHUDWidget::PlayerHUDWidget()
{
	TopPanelWidget = CreateNewObject<PanelWidget>(this);
	if (TopPanelWidget.Get())
	{
		if (auto pBrush = TopPanelWidget->GetBackgroundBrush())
		{
			BrushStyle panelStyle;
			panelStyle.FillPixel = '.';
			panelStyle.FillColorRGB = RenderConstants::DarkGrayPixelColorRGB;

			pBrush->SetBrushStyle(panelStyle);
		}

		TopPanelWidget->GetAlignment().Horizontal = AlignmentSettings::HorizontalAlignment::NoAlignment;
		TopPanelWidget->GetAlignment().Vertical = AlignmentSettings::VerticalAlignment::Top;
		TopPanelWidget->GetAlignment().Stretch = AlignmentSettings::StretchMode::Fill;

		TopPanelWidget->GetLayout().DimensionsOverride.cy = 3;

		Tree.PlaceWidgetOn(TopPanelWidget.Get(), Canvas.Get());
	}

	TopHorizontalBox = CreateNewObject<HorizontalBox>(this);
	if (TopHorizontalBox.Get())
	{
		Tree.PlaceWidgetOn(TopHorizontalBox.Get(), TopPanelWidget.Get());
	}

	ScoreText = CreateNewObject<TextBlock>(this);
	if (ScoreText.Get())
	{
		ScoreText->GetAlignment().Horizontal = AlignmentSettings::HorizontalAlignment::Left;
		ScoreText->GetAlignment().Vertical = AlignmentSettings::VerticalAlignment::Center;
		ScoreText->GetAlignment().Stretch = AlignmentSettings::StretchMode::NoStretch;
		ScoreText->GetAlignment().Padding = { 2, 0, 0, 0 };

		ScoreText->GetText().SetText("Score: 0");
		ScoreText->GetText().SetFontStyle({ 1, FontPrintType::Simple, RenderConstants::LightOrangePixelColorRGB });

		Tree.PlaceWidgetOn(ScoreText.Get(), TopHorizontalBox.Get());
	}

	MenuButton = CreateNewObject<Button>(this);
	if (MenuButton.Get())
	{
		MenuButton->GetAlignment().Horizontal = AlignmentSettings::HorizontalAlignment::Right;
		MenuButton->GetAlignment().Vertical = AlignmentSettings::VerticalAlignment::NoAlignment;
		MenuButton->GetAlignment().Stretch = AlignmentSettings::StretchMode::NoStretch;

		MenuButton->GetAlignment().Padding = { 0, 0, 2, 0 };

		MenuButton->GetLayout().DimensionsOverride.cx = 12;
		MenuButton->GetLayout().DimensionsOverride.cy = 3;

		Tree.PlaceWidgetOn(MenuButton.Get(), TopHorizontalBox.Get());

		MenuButton->OnClickEvent().Subscribe(this, &PlayerHUDWidget::HandleOpenMenuButtonClick);
	}

	MenuButtonText = CreateNewObject<TextBlock>(this);
	if (MenuButtonText.Get())
	{
		MenuButtonText->GetAlignment().Horizontal = AlignmentSettings::HorizontalAlignment::NoAlignment;
		MenuButtonText->GetAlignment().Vertical = AlignmentSettings::VerticalAlignment::NoAlignment;
		MenuButtonText->GetAlignment().Stretch = AlignmentSettings::StretchMode::Fill;

		MenuButtonText->GetText().SetText("Menu");
		MenuButtonText->GetText().SetFontStyle({ 0, FontPrintType::Simple, RenderConstants::LightGrayPixelColorRGB });

		Tree.PlaceWidgetOn(MenuButtonText.Get(), MenuButton.Get());
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

void PlayerHUDWidget::HandleOpenMenuButtonClick(Button* instigator)
{
	MenuOpenClickEvent.Trigger();
}

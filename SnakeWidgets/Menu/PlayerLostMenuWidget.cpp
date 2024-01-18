#include "PlayerLostMenuWidget.h"

#include "Engine/GameWidget/Components/VerticalBox.h"
#include "Engine/GameWidget/Components/PanelWidget.h"
#include "Engine/GameWidget/Components/Button.h"
#include "Engine/GameWidget/Components/TextBlock.h"

#include "Engine/GameObject/GameObjectUtility.h"

PlayerLostMenuWidget::PlayerLostMenuWidget()
{
	SetWidgetModality(true);

	MenuFrameWidget = CreateNewObject<PanelWidget>(this);
	if (MenuFrameWidget.Get())
	{
		if (auto pBrush = MenuFrameWidget->GetBackgroundBrush())
		{
			BrushStyle panelStyle;
			panelStyle.FillPixel = '.';
			panelStyle.FillColorRGB = RenderConstants::DarkGrayPixelColorRGB;

			pBrush->SetBrushStyle(panelStyle);
		}

		MenuFrameWidget->GetAlignment().Horizontal = AlignmentSettings::HorizontalAlignment::Center;
		MenuFrameWidget->GetAlignment().Vertical = AlignmentSettings::VerticalAlignment::Center;
		MenuFrameWidget->GetAlignment().Stretch = AlignmentSettings::StretchMode::NoStretch;

		MenuFrameWidget->GetLayout().DimensionsOverride.cx = 60;
		MenuFrameWidget->GetLayout().DimensionsOverride.cy = 20;

		Tree.PlaceWidgetOn(MenuFrameWidget.Get(), Canvas.Get());
	}

	FrameVerticalBox = CreateNewObject<VerticalBox>(this);
	if (FrameVerticalBox.Get())
	{
		Tree.PlaceWidgetOn(FrameVerticalBox.Get(), MenuFrameWidget.Get());
	}

	CaptionText = CreateNewObject<TextBlock>(this);
	if (CaptionText.Get())
	{
		CaptionText->GetAlignment().Horizontal = AlignmentSettings::HorizontalAlignment::Center;
		CaptionText->GetAlignment().Vertical = AlignmentSettings::VerticalAlignment::Top;
		CaptionText->GetAlignment().Stretch = AlignmentSettings::StretchMode::NoStretch;

		CaptionText->GetAlignment().Padding = { 0, 2, 0, 0 };

		CaptionText->GetText().SetText("You Lost!");
		CaptionText->GetText().SetFontStyle({ 1, FontPrintType::LetterBig, RenderConstants::LightRedPixelColorRGB, '#' });

		Tree.PlaceWidgetOn(CaptionText.Get(), FrameVerticalBox.Get());
	}

	PlayAgainButton = CreateNewObject<Button>(this);
	if (PlayAgainButton.Get())
	{
		PlayAgainButton->GetAlignment().Horizontal = AlignmentSettings::HorizontalAlignment::Center;
		PlayAgainButton->GetAlignment().Vertical = AlignmentSettings::VerticalAlignment::Top;
		PlayAgainButton->GetAlignment().Stretch = AlignmentSettings::StretchMode::NoStretch;

		PlayAgainButton->GetAlignment().Padding = { 0, 3, 0, 0 };

		PlayAgainButton->GetLayout().DimensionsOverride.cx = 20;
		PlayAgainButton->GetLayout().DimensionsOverride.cy = 3;

		Tree.PlaceWidgetOn(PlayAgainButton.Get(), FrameVerticalBox.Get());

		PlayAgainButton->OnClickEvent().Subscribe(this, &PlayerLostMenuWidget::HandlePlayAgainButtonClick);
	}

	PlayAgainButtonText = CreateNewObject<TextBlock>(this);
	if (PlayAgainButtonText.Get())
	{
		PlayAgainButtonText->GetAlignment().Horizontal = AlignmentSettings::HorizontalAlignment::NoAlignment;
		PlayAgainButtonText->GetAlignment().Vertical = AlignmentSettings::VerticalAlignment::NoAlignment;
		PlayAgainButtonText->GetAlignment().Stretch = AlignmentSettings::StretchMode::Fill;

		PlayAgainButtonText->GetText().SetText("Play Again");
		PlayAgainButtonText->GetText().SetFontStyle({ 0, FontPrintType::Simple, RenderConstants::LightGrayPixelColorRGB });

		Tree.PlaceWidgetOn(PlayAgainButtonText.Get(), PlayAgainButton.Get());
	}

	ExitGameButton = GameObject::CloneObject<Button>(PlayAgainButton.Get(), this);
	if (ExitGameButton.Get())
	{
		Tree.PlaceWidgetOn(ExitGameButton.Get(), FrameVerticalBox.Get());

		ExitGameButton->GetAlignment().Padding = { 0, 1, 0, 0 };

		ExitGameButton->OnClickEvent().Subscribe(this, &PlayerLostMenuWidget::HandleExitGameButtonClick);
	}

	ExitGameButtonText = GameObject::CloneObject<TextBlock>(PlayAgainButtonText.Get(), this);
	if (ExitGameButtonText.Get())
	{
		Tree.PlaceWidgetOn(ExitGameButtonText.Get(), ExitGameButton.Get());

		ExitGameButtonText->GetText().SetText("Exit Game");
	}
}

PlayerLostMenuWidget::~PlayerLostMenuWidget()
{
}

void PlayerLostMenuWidget::HandlePlayAgainButtonClick(Button* instigator)
{
	PlayAgainClickEvent.Trigger();
}

void PlayerLostMenuWidget::HandleExitGameButtonClick(Button* instigator)
{
	ExitGameClickEvent.Trigger();
}

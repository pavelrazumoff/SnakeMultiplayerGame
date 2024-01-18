#include "GameMenuWidget.h"

#include "Engine/GameWidget/Components/VerticalBox.h"
#include "Engine/GameWidget/Components/PanelWidget.h"
#include "Engine/GameWidget/Components/Button.h"
#include "Engine/GameWidget/Components/TextBlock.h"

#include "Engine/GameObject/GameObjectUtility.h"

GameMenuWidget::GameMenuWidget()
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

		CaptionText->GetText().SetText("Menu");
		CaptionText->GetText().SetFontStyle({ 1, FontPrintType::LetterBig, RenderConstants::LightYellowPixelColorRGB, '#' });

		Tree.PlaceWidgetOn(CaptionText.Get(), FrameVerticalBox.Get());
	}

	ResumeButton = CreateNewObject<Button>(this);
	if (ResumeButton.Get())
	{
		ResumeButton->GetAlignment().Horizontal = AlignmentSettings::HorizontalAlignment::Center;
		ResumeButton->GetAlignment().Vertical = AlignmentSettings::VerticalAlignment::Top;
		ResumeButton->GetAlignment().Stretch = AlignmentSettings::StretchMode::NoStretch;

		ResumeButton->GetAlignment().Padding = { 0, 3, 0, 0 };

		ResumeButton->GetLayout().DimensionsOverride.cx = 20;
		ResumeButton->GetLayout().DimensionsOverride.cy = 3;

		Tree.PlaceWidgetOn(ResumeButton.Get(), FrameVerticalBox.Get());

		ResumeButton->OnClickEvent().Subscribe(this, &GameMenuWidget::HandleResumeButtonClick);
	}

	ResumeButtonText = CreateNewObject<TextBlock>(this);
	if (ResumeButtonText.Get())
	{
		ResumeButtonText->GetAlignment().Horizontal = AlignmentSettings::HorizontalAlignment::NoAlignment;
		ResumeButtonText->GetAlignment().Vertical = AlignmentSettings::VerticalAlignment::NoAlignment;
		ResumeButtonText->GetAlignment().Stretch = AlignmentSettings::StretchMode::Fill;

		ResumeButtonText->GetText().SetText("Resume");
		ResumeButtonText->GetText().SetFontStyle({ 0, FontPrintType::Simple, RenderConstants::LightGrayPixelColorRGB });

		Tree.PlaceWidgetOn(ResumeButtonText.Get(), ResumeButton.Get());
	}

	ExitGameButton = GameObject::CloneObject<Button>(ResumeButton.Get(), this);
	if (ExitGameButton.Get())
	{
		Tree.PlaceWidgetOn(ExitGameButton.Get(), FrameVerticalBox.Get());

		ExitGameButton->GetAlignment().Padding = { 0, 1, 0, 0 };

		ExitGameButton->OnClickEvent().Subscribe(this, &GameMenuWidget::HandleExitGameButtonClick);
	}

	ExitGameButtonText = GameObject::CloneObject<TextBlock>(ResumeButtonText.Get(), this);
	if (ExitGameButtonText.Get())
	{
		Tree.PlaceWidgetOn(ExitGameButtonText.Get(), ExitGameButton.Get());

		ExitGameButtonText->GetText().SetText("Exit Game");
	}
}

GameMenuWidget::~GameMenuWidget()
{
}

void GameMenuWidget::HandleResumeButtonClick(Button* instigator)
{
	ResumeClickEvent.Trigger();
}

void GameMenuWidget::HandleExitGameButtonClick(Button* instigator)
{
	ExitGameClickEvent.Trigger();
}

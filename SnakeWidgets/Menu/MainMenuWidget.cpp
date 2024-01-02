#include "MainMenuWidget.h"

#include "Engine/GameWidget/Components/VerticalBox.h"
#include "Engine/GameWidget/Components/PanelWidget.h"
#include "Engine/GameWidget/Components/Button.h"
#include "Engine/GameWidget/Components/TextBlock.h"

#include "Engine/GameObject/GameObjectUtility.h"

MainMenuWidget::MainMenuWidget()
{
	MainVerticalBox = CreateNewObject<VerticalBox>(this);
	if (MainVerticalBox.Get())
	{
		Tree.PlaceWidgetOn(MainVerticalBox.Get(), Canvas.Get());
	}

	TopPanelWidget = CreateNewObject<PanelWidget>(this);
	if (TopPanelWidget.Get())
	{
		if (auto pBrush = TopPanelWidget->GetBackgroundBrush())
		{
			BrushStyle panelStyle;
			panelStyle.FillPixel = '*';
			panelStyle.FillColorRGB = RenderConstants::GreenPixelColorRGB;

			pBrush->SetBrushStyle(panelStyle);
		}

		TopPanelWidget->GetAlignment().Horizontal = AlignmentSettings::HorizontalAlignment::NoAlignment;
		TopPanelWidget->GetAlignment().Vertical = AlignmentSettings::VerticalAlignment::Top;
		TopPanelWidget->GetAlignment().Stretch = AlignmentSettings::StretchMode::Fill;

		TopPanelWidget->GetLayout().DimensionsOverride.cy = 2;

		Tree.PlaceWidgetOn(TopPanelWidget.Get(), MainVerticalBox.Get());
	}

	WelcomeCaptionText = CreateNewObject<TextBlock>(this);
	if (WelcomeCaptionText.Get())
	{
		WelcomeCaptionText->GetAlignment().Horizontal = AlignmentSettings::HorizontalAlignment::Center;
		WelcomeCaptionText->GetAlignment().Vertical = AlignmentSettings::VerticalAlignment::Center;
		WelcomeCaptionText->GetAlignment().Stretch = AlignmentSettings::StretchMode::NoStretch;

		// We don't yet have dirty size calc based on text content.
		WelcomeCaptionText->GetLayout().DimensionsOverride.cx = 31;//200;
		WelcomeCaptionText->GetLayout().DimensionsOverride.cy = 7;

		WelcomeCaptionText->GetText().SetText("Welcome!");
		WelcomeCaptionText->GetText().SetFontStyle({ 1, RenderConstants::LightGrayPixelColorRGB });

		Tree.PlaceWidgetOn(WelcomeCaptionText.Get(), MainVerticalBox.Get());
	}

	StartGameButton = CreateNewObject<Button>(this);
	if (StartGameButton.Get())
	{
		StartGameButton->GetAlignment().Horizontal = AlignmentSettings::HorizontalAlignment::Center;
		StartGameButton->GetAlignment().Vertical = AlignmentSettings::VerticalAlignment::Top;
		StartGameButton->GetAlignment().Stretch = AlignmentSettings::StretchMode::NoStretch;

		StartGameButton->GetLayout().DimensionsOverride.cx = 12;
		StartGameButton->GetLayout().DimensionsOverride.cy = 3;

		Tree.PlaceWidgetOn(StartGameButton.Get(), MainVerticalBox.Get());
	}

	StartGameButtonText = CreateNewObject<TextBlock>(this);
	if (StartGameButtonText.Get())
	{
		StartGameButtonText->GetAlignment().Horizontal = AlignmentSettings::HorizontalAlignment::NoAlignment;
		StartGameButtonText->GetAlignment().Vertical = AlignmentSettings::VerticalAlignment::NoAlignment;
		StartGameButtonText->GetAlignment().Stretch = AlignmentSettings::StretchMode::Fill;

		StartGameButtonText->GetText().SetText("Start Game");
		StartGameButtonText->GetText().SetFontStyle({ 0, RenderConstants::LightGrayPixelColorRGB });

		Tree.PlaceWidgetOn(StartGameButtonText.Get(), StartGameButton.Get());
	}

	BottomPanelWidget = GameObject::CloneObject(TopPanelWidget.Get());
	if (BottomPanelWidget.Get())
	{
		if (auto pBrush = BottomPanelWidget->GetBackgroundBrush())
		{
			BrushStyle panelStyle = pBrush->GetBrushStyle();
			panelStyle.FillColorRGB = RenderConstants::RedPixelColorRGB;

			pBrush->SetBrushStyle(panelStyle);
		}

		BottomPanelWidget->GetAlignment().Vertical = AlignmentSettings::VerticalAlignment::Bottom;
	
		Tree.PlaceWidgetOn(BottomPanelWidget.Get(), MainVerticalBox.Get());
	}
}

MainMenuWidget::~MainMenuWidget()
{
}

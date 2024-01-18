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
		TopPanelWidget->GetAlignment().Stretch = AlignmentSettings::StretchMode::NoStretch;

		TopPanelWidget->GetLayout().DimensionsOverride.cy = 2;

		Tree.PlaceWidgetOn(TopPanelWidget.Get(), MainVerticalBox.Get());
	}

	WelcomeCaptionText = CreateNewObject<TextBlock>(this);
	if (WelcomeCaptionText.Get())
	{
		WelcomeCaptionText->GetAlignment().Horizontal = AlignmentSettings::HorizontalAlignment::Center;
		WelcomeCaptionText->GetAlignment().Vertical = AlignmentSettings::VerticalAlignment::Bottom;
		WelcomeCaptionText->GetAlignment().Stretch = AlignmentSettings::StretchMode::Fill;

		WelcomeCaptionText->GetAlignment().Padding = { 0, 0, 0, 2 };

		WelcomeCaptionText->GetText().SetText("The Snake Game");
		WelcomeCaptionText->GetText().SetFontStyle({ 1, FontPrintType::LetterBig, RenderConstants::LightGrayPixelColorRGB });

		Tree.PlaceWidgetOn(WelcomeCaptionText.Get(), MainVerticalBox.Get());
	}

	StartGameButton = CreateNewObject<Button>(this);
	if (StartGameButton.Get())
	{
		StartGameButton->GetAlignment().Horizontal = AlignmentSettings::HorizontalAlignment::Center;
		StartGameButton->GetAlignment().Vertical = AlignmentSettings::VerticalAlignment::Top;
		StartGameButton->GetAlignment().Stretch = AlignmentSettings::StretchMode::NoStretch;

		StartGameButton->GetAlignment().Padding = { 0, 0, 0, 1 };

		StartGameButton->GetLayout().DimensionsOverride.cx = 14;
		StartGameButton->GetLayout().DimensionsOverride.cy = 3;

		Tree.PlaceWidgetOn(StartGameButton.Get(), MainVerticalBox.Get());

		StartGameButton->OnClickEvent().Subscribe(this, &MainMenuWidget::HandleStartGameButtonClick);
	}

	StartGameButtonText = CreateNewObject<TextBlock>(this);
	if (StartGameButtonText.Get())
	{
		StartGameButtonText->GetAlignment().Horizontal = AlignmentSettings::HorizontalAlignment::NoAlignment;
		StartGameButtonText->GetAlignment().Vertical = AlignmentSettings::VerticalAlignment::NoAlignment;
		StartGameButtonText->GetAlignment().Stretch = AlignmentSettings::StretchMode::Fill;

		StartGameButtonText->GetText().SetText("Play");
		StartGameButtonText->GetText().SetFontStyle({ 0, FontPrintType::Simple, RenderConstants::LightGrayPixelColorRGB });

		Tree.PlaceWidgetOn(StartGameButtonText.Get(), StartGameButton.Get());
	}

	ExitGameButton = GameObject::CloneObject<Button>(StartGameButton.Get(), this);
	if (ExitGameButton.Get())
	{
		ExitGameButton->GetAlignment().Stretch = AlignmentSettings::StretchMode::Fill;

		Tree.PlaceWidgetOn(ExitGameButton.Get(), MainVerticalBox.Get());

		ExitGameButton->OnClickEvent().Subscribe(this, &MainMenuWidget::HandleExitGameButtonClick);
	}

	ExitGameButtonText = GameObject::CloneObject<TextBlock>(StartGameButtonText.Get(), this);
	if (ExitGameButtonText.Get())
	{
		Tree.PlaceWidgetOn(ExitGameButtonText.Get(), ExitGameButton.Get());

		ExitGameButtonText->GetText().SetText("Exit Game");
	}

	BottomPanelWidget = GameObject::CloneObject(TopPanelWidget.Get(), this);
	if (BottomPanelWidget.Get())
	{
		if (auto pBrush = BottomPanelWidget->GetBackgroundBrush())
		{
			BrushStyle panelStyle = pBrush->GetBrushStyle();
			panelStyle.FillColorRGB = RenderConstants::RedPixelColorRGB;

			pBrush->SetBrushStyle(panelStyle);
		}

		BottomPanelWidget->GetAlignment().Vertical = AlignmentSettings::VerticalAlignment::Bottom;
		BottomPanelWidget->GetAlignment().Stretch = AlignmentSettings::StretchMode::NoStretch;
	
		Tree.PlaceWidgetOn(BottomPanelWidget.Get(), MainVerticalBox.Get());
	}
}

MainMenuWidget::~MainMenuWidget()
{
}

void MainMenuWidget::HandleStartGameButtonClick(Button* instigator)
{
	ResponseEvent.Trigger(static_cast<uint8_t>(MainMenuResponse::StartGame));
}

void MainMenuWidget::HandleExitGameButtonClick(Button* instigator)
{
	ResponseEvent.Trigger(static_cast<uint8_t>(MainMenuResponse::ExitGame));
}

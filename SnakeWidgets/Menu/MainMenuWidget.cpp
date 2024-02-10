#include "MainMenuWidget.h"

#include "Engine/GameWidget/Components/VerticalBox.h"
#include "Engine/GameWidget/Components/PanelWidget.h"
#include "Engine/GameWidget/Components/Button.h"
#include "Engine/GameWidget/Components/TextBlock.h"
#include "Engine/GameWidget/Components/PageContainer.h"

#include "Engine/GameObject/GameObjectUtility.h"

#include "SnakeWidgets/WidgetsFactory.h"

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

	MainMenuContent = CreateNewObject<PageContainerWidget>(this);
	if (MainMenuContent.Get())
	{
		MainMenuContent->GetAlignment().Horizontal = AlignmentSettings::HorizontalAlignment::NoAlignment;
		MainMenuContent->GetAlignment().Vertical = AlignmentSettings::VerticalAlignment::NoAlignment;
		MainMenuContent->GetAlignment().Stretch = AlignmentSettings::StretchMode::Fill;

		Tree.PlaceWidgetOn(MainMenuContent.Get(), MainVerticalBox.Get());
	}

	/** Welcome page content. */

	VerticalBox* WelcomePageVerticalBox = CreateNewObject<VerticalBox>(this);
	if (WelcomePageVerticalBox)
		Tree.PlaceWidgetOn(WelcomePageVerticalBox, MainMenuContent.Get());

	TextBlock* WelcomeCaptionText = CreateNewObject<TextBlock>(this);
	if (WelcomeCaptionText)
	{
		WelcomeCaptionText->GetAlignment().Horizontal = AlignmentSettings::HorizontalAlignment::Center;
		WelcomeCaptionText->GetAlignment().Vertical = AlignmentSettings::VerticalAlignment::Bottom;
		WelcomeCaptionText->GetAlignment().Stretch = AlignmentSettings::StretchMode::Fill;

		WelcomeCaptionText->GetAlignment().Padding = { 0, 0, 0, 2 };

		WelcomeCaptionText->GetText().SetText("The Snake Game");
		WelcomeCaptionText->GetText().SetFontStyle({ 1, FontPrintType::LetterBig, RenderConstants::LightGrayPixelColorRGB });

		Tree.PlaceWidgetOn(WelcomeCaptionText, WelcomePageVerticalBox);
	}

	if (Button* StartGameButton = WidgetsFactory::CreateMenuButton("Single Play", { 14, 3 }, this, WelcomePageVerticalBox))
		StartGameButton->OnClickEvent().Subscribe(this, &MainMenuWidget::HandleStartGameButtonClick);

	if (Button* MultiplayerButton = WidgetsFactory::CreateMenuButton("Multiplayer", { 14, 3 }, this, WelcomePageVerticalBox))
		MultiplayerButton->OnClickEvent().Subscribe(this, &MainMenuWidget::HandleMultiplayerButtonClick);

	if (Button* ExitGameButton = WidgetsFactory::CreateMenuButton("Exit Game", { 14, 3 }, this, WelcomePageVerticalBox))
	{
		ExitGameButton->GetAlignment().Stretch = AlignmentSettings::StretchMode::Fill;
		ExitGameButton->OnClickEvent().Subscribe(this, &MainMenuWidget::HandleExitGameButtonClick);
	}

	/** Multiplayer page content. */

	VerticalBox* MultiplayerPageVerticalBox = CreateNewObject<VerticalBox>(this);
	if (MultiplayerPageVerticalBox)
		Tree.PlaceWidgetOn(MultiplayerPageVerticalBox, MainMenuContent.Get());

	if (Button* CreateServerButton = WidgetsFactory::CreateMenuButton("Create Server", { 15, 3 }, this, MultiplayerPageVerticalBox))
	{
		CreateServerButton->GetAlignment().Vertical = AlignmentSettings::VerticalAlignment::Bottom;
		CreateServerButton->GetAlignment().Stretch = AlignmentSettings::StretchMode::Fill;
		CreateServerButton->OnClickEvent().Subscribe(this, &MainMenuWidget::HandleCreateServerButtonClick);
	}

	if (Button* JoinServerButton = WidgetsFactory::CreateMenuButton("Join Server", { 15, 3 }, this, MultiplayerPageVerticalBox))
		JoinServerButton->OnClickEvent().Subscribe(this, &MainMenuWidget::HandleJoinServerButtonClick);

	if (Button* ReturnButton = WidgetsFactory::CreateMenuButton("Return", { 15, 3 }, this, MultiplayerPageVerticalBox))
	{
		ReturnButton->GetAlignment().Vertical = AlignmentSettings::VerticalAlignment::Top;
		ReturnButton->GetAlignment().Stretch = AlignmentSettings::StretchMode::Fill;
		ReturnButton->OnClickEvent().Subscribe(this, &MainMenuWidget::HandleReturnToMainMenuButtonClick);
	}

	/** Create server page content. */

	VerticalBox* CreateServerPageVerticalBox = CreateNewObject<VerticalBox>(this);
	if (CreateServerPageVerticalBox)
		Tree.PlaceWidgetOn(CreateServerPageVerticalBox, MainMenuContent.Get());

	// TODO: Server (room) name.
	// Max number of players.
	// Difficulty level.
	// etc.

	if (Button* LocalServerButton = WidgetsFactory::CreateMenuButton("Local Server", { 15, 3 }, this, CreateServerPageVerticalBox))
	{
		LocalServerButton->GetAlignment().Vertical = AlignmentSettings::VerticalAlignment::Bottom;
		LocalServerButton->GetAlignment().Stretch = AlignmentSettings::StretchMode::Fill;

		LocalServerButton->OnClickEvent().Subscribe(this, &MainMenuWidget::HandleMakeLocalServerClick);
	}

	if (Button* ReturnButton = WidgetsFactory::CreateMenuButton("Return", { 15, 3 }, this, CreateServerPageVerticalBox))
	{
		ReturnButton->GetAlignment().Vertical = AlignmentSettings::VerticalAlignment::Top;
		ReturnButton->GetAlignment().Stretch = AlignmentSettings::StretchMode::Fill;
		ReturnButton->OnClickEvent().Subscribe(this, &MainMenuWidget::HandleReturnToMultiplayerPageClick);
	}

	/** Join server page content. */

	// TODO: List of available servers.
	// Enter server address.

	VerticalBox* JoinServerPageVerticalBox = CreateNewObject<VerticalBox>(this);
	if (JoinServerPageVerticalBox)
		Tree.PlaceWidgetOn(JoinServerPageVerticalBox, MainMenuContent.Get());

	if (Button* JoinLocalhostButton = WidgetsFactory::CreateMenuButton("Join LocalHost", { 20, 3 }, this, JoinServerPageVerticalBox))
	{
		JoinLocalhostButton->GetAlignment().Vertical = AlignmentSettings::VerticalAlignment::Bottom;
		JoinLocalhostButton->GetAlignment().Stretch = AlignmentSettings::StretchMode::Fill;

		JoinLocalhostButton->OnClickEvent().Subscribe(this, &MainMenuWidget::JoinLocalHostClick);
	}

	if (Button* ReturnButton = WidgetsFactory::CreateMenuButton("Return", { 15, 3 }, this, JoinServerPageVerticalBox))
	{
		ReturnButton->GetAlignment().Vertical = AlignmentSettings::VerticalAlignment::Top;
		ReturnButton->GetAlignment().Stretch = AlignmentSettings::StretchMode::Fill;
		ReturnButton->OnClickEvent().Subscribe(this, &MainMenuWidget::HandleReturnToMultiplayerPageClick);
	}

	/**  */

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

void MainMenuWidget::OpenPage(ContentPageType page)
{
	switch (page)
	{
		case ContentPageType::Welcome:
			MainMenuContent->SetCurrentPageIndex(0);
			break;
		case ContentPageType::Multiplayer:
			MainMenuContent->SetCurrentPageIndex(1);
			break;
		case ContentPageType::CreateServer:
			MainMenuContent->SetCurrentPageIndex(2);
			break;
		case ContentPageType::JoinServer:
			MainMenuContent->SetCurrentPageIndex(3);
			break;
	}
}

/*
	Welcome page content.
*/

void MainMenuWidget::HandleStartGameButtonClick(Button* instigator)
{
	ResponseEvent.Trigger(static_cast<uint8_t>(MainMenuResponse::StartGame));
}

void MainMenuWidget::HandleMultiplayerButtonClick(Button* instigator)
{
	ResponseEvent.Trigger(static_cast<uint8_t>(MainMenuResponse::Multiplayer));
}

void MainMenuWidget::HandleExitGameButtonClick(Button* instigator)
{
	ResponseEvent.Trigger(static_cast<uint8_t>(MainMenuResponse::ExitGame));
}

/*
	Multiplayer page content.
*/

void MainMenuWidget::HandleCreateServerButtonClick(Button* instigator)
{
	ResponseEvent.Trigger(static_cast<uint8_t>(MainMenuResponse::CreateServer));
}

void MainMenuWidget::HandleJoinServerButtonClick(Button* instigator)
{
	ResponseEvent.Trigger(static_cast<uint8_t>(MainMenuResponse::JoinServer));
}

void MainMenuWidget::HandleReturnToMainMenuButtonClick(Button* instigator)
{
	ResponseEvent.Trigger(static_cast<uint8_t>(MainMenuResponse::ReturnToMainMenu));
}

/*
	Create server page content.
*/

void MainMenuWidget::HandleMakeLocalServerClick(Button* instigator)
{
	ResponseEvent.Trigger(static_cast<uint8_t>(MainMenuResponse::MakeLocalServer));
}

void MainMenuWidget::HandleReturnToMultiplayerPageClick(Button* instigator)
{
	ResponseEvent.Trigger(static_cast<uint8_t>(MainMenuResponse::Multiplayer));
}

/*
	Join server page content.
*/

void MainMenuWidget::JoinLocalHostClick(Button* instigator)
{
	ResponseEvent.Trigger(static_cast<uint8_t>(MainMenuResponse::JoinLocalHost));
}

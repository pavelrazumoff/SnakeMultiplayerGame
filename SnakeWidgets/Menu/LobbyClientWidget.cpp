#include "LobbyClientWidget.h"

#include "Engine/GameWidget/Components/VerticalBox.h"
#include "Engine/GameWidget/Components/HorizontalBox.h"

#include "Engine/GameWidget/Components/TextBlock.h"
#include "Engine/GameWidget/Components/TextEditBox.h"

#include "Engine/GameWidget/Components/Button.h"

#include "Engine/GameObject/GameObjectUtility.h"
#include "Engine/Player/PlayerManager.h"

LobbyClientWidget::LobbyClientWidget()
{
	VerticalBox* MenuVerticalBox = CreateNewObject<VerticalBox>(this);
	if (MenuVerticalBox)
		Tree.PlaceWidgetOn(MenuVerticalBox, Canvas.Get());

	if (TextBlock* CaptionText = CreateNewObject<TextBlock>(this))
	{
		CaptionText->GetAlignment().Horizontal = AlignmentSettings::HorizontalAlignment::Center;
		CaptionText->GetAlignment().Vertical = AlignmentSettings::VerticalAlignment::Top;
		CaptionText->GetAlignment().Stretch = AlignmentSettings::StretchMode::Fill;

		CaptionText->GetAlignment().Padding = { 0, 0, 0, 2 };

		CaptionText->GetText().SetText("Lobby (client)");
		CaptionText->GetText().SetFontStyle({ 1, FontPrintType::LetterBig, RenderConstants::LightOrangePixelColorRGB });

		Tree.PlaceWidgetOn(CaptionText, MenuVerticalBox);
	}

	HorizontalBox* PlayerNameBox = CreateNewObject<HorizontalBox>(this);
	if (PlayerNameBox)
	{
		PlayerNameBox->GetAlignment().Horizontal = AlignmentSettings::HorizontalAlignment::Center;
		PlayerNameBox->GetAlignment().Vertical = AlignmentSettings::VerticalAlignment::Top;
		PlayerNameBox->GetAlignment().Stretch = AlignmentSettings::StretchMode::NoStretch;

		PlayerNameBox->GetAlignment().Padding = { 0, 2, 0, 0 };

		Tree.PlaceWidgetOn(PlayerNameBox, MenuVerticalBox);
	}

	if (TextBlock* PlayerNameCaption = CreateNewObject<TextBlock>(this))
	{
		PlayerNameCaption->GetAlignment().Horizontal = AlignmentSettings::HorizontalAlignment::Right;
		PlayerNameCaption->GetAlignment().Vertical = AlignmentSettings::VerticalAlignment::NoAlignment;
		PlayerNameCaption->GetAlignment().Stretch = AlignmentSettings::StretchMode::Fill;

		PlayerNameCaption->GetAlignment().Padding = { 0, 0, 1, 0 };

		PlayerNameCaption->GetText().SetText("Enter your name: ");
		PlayerNameCaption->GetText().SetFontStyle({ 1, FontPrintType::Simple, RenderConstants::DarkGreenPixelColorRGB });

		Tree.PlaceWidgetOn(PlayerNameCaption, PlayerNameBox);
	}

	EditNameBox = CreateNewObject<TextEditBox>(this);
	if (EditNameBox.Get())
	{
		EditNameBox->GetAlignment().Horizontal = AlignmentSettings::HorizontalAlignment::Left;
		EditNameBox->GetAlignment().Vertical = AlignmentSettings::VerticalAlignment::NoAlignment;
		EditNameBox->GetAlignment().Stretch = AlignmentSettings::StretchMode::Fill;

		EditNameBox->GetAlignment().Padding = { 0, 0, 0, 0 };

		EditNameBox->GetText().SetText("default name");
		EditNameBox->GetText().SetFontStyle({ 1, FontPrintType::Simple, RenderConstants::LightGreenPixelColorRGB });

		Tree.PlaceWidgetOn(EditNameBox.Get(), PlayerNameBox);
	}

	Button* ReadyButton = CreateNewObject<Button>(this);
	if (ReadyButton)
	{
		ReadyButton->GetAlignment().Horizontal = AlignmentSettings::HorizontalAlignment::Center;
		ReadyButton->GetAlignment().Vertical = AlignmentSettings::VerticalAlignment::Top;
		ReadyButton->GetAlignment().Stretch = AlignmentSettings::StretchMode::Fill;

		ReadyButton->GetAlignment().Padding = { 0, 1, 0, 0 };

		ReadyButton->GetLayout().DimensionsOverride.cx = 18;
		ReadyButton->GetLayout().DimensionsOverride.cy = 3;

		Tree.PlaceWidgetOn(ReadyButton, MenuVerticalBox);

		ReadyButton->OnClickEvent().Subscribe(this, &LobbyClientWidget::HandleReady2PlayButtonClick);
	}

	if (TextBlock* ReadyButtonText = CreateNewObject<TextBlock>(this))
	{
		ReadyButtonText->GetAlignment().Horizontal = AlignmentSettings::HorizontalAlignment::NoAlignment;
		ReadyButtonText->GetAlignment().Vertical = AlignmentSettings::VerticalAlignment::NoAlignment;
		ReadyButtonText->GetAlignment().Stretch = AlignmentSettings::StretchMode::Fill;

		ReadyButtonText->GetText().SetText("Ready to play");
		ReadyButtonText->GetText().SetFontStyle({ 1, FontPrintType::Simple, RenderConstants::LightGrayPixelColorRGB });

		Tree.PlaceWidgetOn(ReadyButtonText, ReadyButton);
	}
}

void LobbyClientWidget::HandleReady2PlayButtonClick(Button* instigator)
{
	if (!EditNameBox.IsValid()) return;

	auto& playerName = EditNameBox->GetText().GetText();

	if (auto playerState = PlayerManager::GetInstance().GetPlayerState())
		playerState->SetPlayerName(playerName.c_str());
	
	// TODO: Mark this player as ready. Block all input for this client until the game starts.
}

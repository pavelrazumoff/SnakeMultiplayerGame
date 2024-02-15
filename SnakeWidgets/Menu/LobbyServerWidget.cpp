#include "LobbyServerWidget.h"

#include "Engine/GameWidget/Components/VerticalBox.h"
#include "Engine/GameWidget/Components/TextBlock.h"

#include "Engine/Player/PlayerManager.h"
#include "Engine/GameWidget/GameWidgetManager.h"

LobbyServerWidget::LobbyServerWidget()
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

		CaptionText->GetText().SetText("Lobby (server)");
		CaptionText->GetText().SetFontStyle({ 1, FontPrintType::LetterBig, RenderConstants::LightBluePixelColorRGB });

		Tree.PlaceWidgetOn(CaptionText, MenuVerticalBox);
	}

	PlayerListVerticalBox = CreateNewObject<VerticalBox>(this);
	if (PlayerListVerticalBox.Get())
	{
		PlayerListVerticalBox->GetAlignment().Horizontal = AlignmentSettings::HorizontalAlignment::NoAlignment;

		Tree.PlaceWidgetOn(PlayerListVerticalBox.Get(), MenuVerticalBox);
	}

	PlayerManager::GetInstance().OnPlayerListChangeEvent().Subscribe(this, &LobbyServerWidget::HandlePlayerListChanged);
}

void LobbyServerWidget::HandlePlayerListChanged()
{
	if (!PlayerListVerticalBox.IsValid()) return;

	PlayerListVerticalBox->ClearChildren();

	const uint32_t numPlayers = PlayerManager::GetInstance().GetPlayerCount();
	for (uint32_t i = 0; i < numPlayers; ++i)
	{
		const auto pPlayerState = PlayerManager::GetInstance().GetPlayerState(i);
		if (!pPlayerState) { DebugEngineTrap(); continue; }

		if (TextBlock* PlayerNameText = CreateNewObject<TextBlock>(this))
		{
			PlayerNameText->GetAlignment().Horizontal = AlignmentSettings::HorizontalAlignment::Center;
			PlayerNameText->GetAlignment().Vertical = AlignmentSettings::VerticalAlignment::Top;
			PlayerNameText->GetAlignment().Stretch = AlignmentSettings::StretchMode::NoStretch;

			PlayerNameText->GetAlignment().Padding = { 2, 0, 2, 1 };

			PlayerNameText->GetText().SetText(pPlayerState->GetPlayerName());
			PlayerNameText->GetText().SetFontStyle({ 1, FontPrintType::Simple, RenderConstants::LightBluePixelColorRGB });

			Tree.PlaceWidgetOn(PlayerNameText, PlayerListVerticalBox.Get());
		}
	}

	// TODO: Move it inside Tree.
	GameWidgetManager::GetInstance().RequestWidgetReconstruction(this);
}

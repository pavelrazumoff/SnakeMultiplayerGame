#include "LobbyServerWidget.h"

#include "Engine/GameWidget/Components/VerticalBox.h"
#include "Engine/GameWidget/Components/TextBlock.h"

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
		PlayerListVerticalBox->GetAlignment().Horizontal = AlignmentSettings::HorizontalAlignment::Center;

		Tree.PlaceWidgetOn(PlayerListVerticalBox.Get(), MenuVerticalBox);
	}
}

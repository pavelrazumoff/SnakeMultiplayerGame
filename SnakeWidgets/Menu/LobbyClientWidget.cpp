#include "LobbyClientWidget.h"

#include "Engine/GameWidget/Components/VerticalBox.h"
#include "Engine/GameWidget/Components/HorizontalBox.h"

#include "Engine/GameWidget/Components/TextBlock.h"
#include "Engine/GameWidget/Components/TextEditBox.h"

#include "Engine/GameObject/GameObjectUtility.h"

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
		PlayerNameBox->GetAlignment().Stretch = AlignmentSettings::StretchMode::Fill;

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

	if (TextEditBox* EditNameBox = CreateNewObject<TextEditBox>(this))
	{
		EditNameBox->GetAlignment().Horizontal = AlignmentSettings::HorizontalAlignment::Left;
		EditNameBox->GetAlignment().Vertical = AlignmentSettings::VerticalAlignment::NoAlignment;
		EditNameBox->GetAlignment().Stretch = AlignmentSettings::StretchMode::Fill;

		EditNameBox->GetAlignment().Padding = { 0, 0, 0, 0 };

		EditNameBox->GetText().SetText("default name");
		EditNameBox->GetText().SetFontStyle({ 1, FontPrintType::Simple, RenderConstants::LightGreenPixelColorRGB });

		Tree.PlaceWidgetOn(EditNameBox, PlayerNameBox);
	}
}

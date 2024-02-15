#include "LobbyClientWidget.h"

#include "Engine/GameWidget/Components/VerticalBox.h"
#include "Engine/GameWidget/Components/TextBlock.h"

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
}

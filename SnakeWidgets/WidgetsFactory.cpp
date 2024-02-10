#include "WidgetsFactory.h"

#include "Engine/GameWidget/Components/UserWidget.h"
#include "Engine/GameWidget/Components/Button.h"
#include "Engine/GameWidget/Components/TextBlock.h"

Button* WidgetsFactory::CreateMenuButton(const std::string& caption, TEX_SIZE buttonSize, UserWidget* Owner, GameWidget* ParentWidget)
{
	if (!Owner) { DebugEngineTrap(); return nullptr; }

	Button* NewButton = CreateNewObject<Button>(Owner);
	if (NewButton)
	{
		NewButton->GetAlignment().Horizontal = AlignmentSettings::HorizontalAlignment::Center;
		NewButton->GetAlignment().Vertical = AlignmentSettings::VerticalAlignment::Top;
		NewButton->GetAlignment().Stretch = AlignmentSettings::StretchMode::NoStretch;

		NewButton->GetAlignment().Padding = { 0, 0, 0, 1 };

		NewButton->GetLayout().DimensionsOverride = buttonSize;

		Owner->GetWidgetTree().PlaceWidgetOn(NewButton, ParentWidget);
	}

	TextBlock* NewButtonText = CreateNewObject<TextBlock>(Owner);
	if (NewButtonText)
	{
		NewButtonText->GetAlignment().Horizontal = AlignmentSettings::HorizontalAlignment::NoAlignment;
		NewButtonText->GetAlignment().Vertical = AlignmentSettings::VerticalAlignment::NoAlignment;
		NewButtonText->GetAlignment().Stretch = AlignmentSettings::StretchMode::Fill;

		NewButtonText->GetText().SetText(caption);
		NewButtonText->GetText().SetFontStyle({ 0, FontPrintType::Simple, RenderConstants::LightGrayPixelColorRGB });

		Owner->GetWidgetTree().PlaceWidgetOn(NewButtonText, NewButton);
	}

	return NewButton;
}

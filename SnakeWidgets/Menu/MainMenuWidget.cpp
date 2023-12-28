#include "MainMenuWidget.h"

#include "Engine/GameWidget/Components/VerticalBox.h"
#include "Engine/GameWidget/Components/PanelWidget.h"
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

			pBrush->SetBrushStyle(panelStyle);
		}

		TopPanelWidget->GetAlignment().Horizontal = AlignmentSettings::HorizontalAlignment::NoAlignment;
		TopPanelWidget->GetAlignment().Vertical = AlignmentSettings::VerticalAlignment::Top;
		TopPanelWidget->GetAlignment().Stretch = AlignmentSettings::StretchMode::Fill;

		TopPanelWidget->GetLayout().DimensionsOverride.cy = 2;

		Tree.PlaceWidgetOn(TopPanelWidget.Get(), MainVerticalBox.Get());
	}

	BottomPanelWidget = GameObject::CloneObject(TopPanelWidget.Get());
	if (BottomPanelWidget.Get())
	{
		BottomPanelWidget->GetAlignment().Vertical = AlignmentSettings::VerticalAlignment::Bottom;
	
		Tree.PlaceWidgetOn(BottomPanelWidget.Get(), MainVerticalBox.Get());
	}
}

MainMenuWidget::~MainMenuWidget()
{
}

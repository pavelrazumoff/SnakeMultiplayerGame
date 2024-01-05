#include "SnakePawn.h"

#include "Core/RenderTextureLibrary.h"
#include "Engine/Log/Logger.h"

SnakePawn::SnakePawn()
{
	std::shared_ptr<RCTexture> imgHead = std::make_shared<RCTexture>(2, 2);
	RenderTextureLibrary::FillTexture(imgHead.get(), '*');
	RenderTextureLibrary::FillTextureColor(imgHead.get(), RenderConstants::GreenPixelColorRGB);

	HeadImageComponent.SetImage(imgHead);
	HeadImageComponent.SetImageSize({ 2.0f, 2.0f });

	AddObjectComponent(&HeadImageComponent);

	// Input Action bindings.
	{
		MoveUpDownAction.Inputs['W'].AxisValue = -1.0f;
		MoveUpDownAction.Inputs['S'].AxisValue = 1.0f;

		InputPawnComponent.BindAction(&MoveUpDownAction, this, &SnakePawn::MoveUpDown_Triggered);

		MoveLeftRightAction.Inputs['A'].AxisValue = -1.0f;
		MoveLeftRightAction.Inputs['D'].AxisValue = 1.0f;

		InputPawnComponent.BindAction(&MoveLeftRightAction, this, &SnakePawn::MoveLeftRight_Triggered);
	}

	MovementPawnComponent.SetMovementSpeed({ 15.0f, 10.0f });
}

SnakePawn::~SnakePawn()
{
}

/*
	Input Action bindings.
*/

void SnakePawn::MoveUpDown_Triggered(InputValue actionValue)
{
	LV_VECTOR dirVec = { 0.0f, actionValue.AxisValue };
	AddMovement(dirVec, 1.0f);
}

void SnakePawn::MoveLeftRight_Triggered(InputValue actionValue)
{
	LV_VECTOR dirVec = { actionValue.AxisValue, 0.0f };
	AddMovement(dirVec, 1.0f);
}

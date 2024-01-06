#include "SnakePawn.h"

#include "Core/RenderTextureLibrary.h"
#include "Engine/Log/Logger.h"

SnakePawn::SnakePawn()
{
	std::shared_ptr<RCTexture> imgHead = std::make_shared<RCTexture>(1, 1);
	RenderTextureLibrary::FillTexture(imgHead.get(), '*');
	RenderTextureLibrary::FillTextureColor(imgHead.get(), RenderConstants::GreenPixelColorRGB);

	std::shared_ptr<RCTexture> imgTail(imgHead->CopyTexture());
	RenderTextureLibrary::FillTextureColor(imgTail.get(), RenderConstants::BluePixelColorRGB);

	// Head.
	{
		HeadImageComponent.SetImage(imgHead);
		HeadImageComponent.SetImageSize({ 1.0f, 1.0f });

		AddObjectComponent(&HeadImageComponent);
	}

	// Tail.
	{
		BodyImageComponent.SetImage(imgTail);
		BodyImageComponent.SetImageSize({ 1.0f, 1.0f });

		BodyImageComponent.SetUseAbsoluteLocation(true);

		AddObjectComponent(&BodyImageComponent);
	}

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

void SnakePawn::BeginPlay()
{
	Inherited::BeginPlay();

	LV_COORD tailLocation = HeadImageComponent.GetSceneLocation() - LV_COORD(0.0f, -1.0f);
	BodyImageComponent.SetAbsoluteLocation(tailLocation);
	bodyPoints.push_back(tailLocation);
}

void SnakePawn::Update(float DeltaTime)
{
	Inherited::Update(DeltaTime);

	UpdateBodyMovement(DeltaTime);
}

void SnakePawn::Render(RCTexture* RenderTargetTexture)
{
	Inherited::Render(RenderTargetTexture);

	// For each body part draw the same BodyImageComponent on each of its location.

	LV_COORD savedFirstBodyLoc = BodyImageComponent.GetSceneLocation();
	for (int i = 1; i < bodyPoints.size(); ++i)
	{
		BodyImageComponent.SetAbsoluteLocation(bodyPoints[i]);
		BodyImageComponent.DrawComponent(RenderTargetTexture);
	}

	BodyImageComponent.SetAbsoluteLocation(savedFirstBodyLoc);
}

/*
	Input Action bindings.
*/

void SnakePawn::MoveUpDown_Triggered(InputValue actionValue)
{
	// If we already move vertically - don't change direction.
	if (MovingDirection.y != 0.0f) return;

	MovingDirection = { 0.0f, actionValue.AxisValue };
}

void SnakePawn::MoveLeftRight_Triggered(InputValue actionValue)
{
	// If we already move horizontally - don't change direction.
	if (MovingDirection.x != 0.0f) return;

	MovingDirection = { actionValue.AxisValue, 0.0f };
}

void SnakePawn::UpdateBodyMovement(float DeltaTime)
{
	if (MovingDirection == LV_VECTOR::Zero()) return;
	
	if (SpawnNewBodyPartTimer > 3.0f)
	{
		SpawnNewBodyPartTimer = 0.0f;

		LV_COORD localHeadLoc = bodyPoints.size() > 1 ? bodyPoints[bodyPoints.size() - 2] : HeadImageComponent.GetSceneLocation();
		LV_VECTOR newBodyPartDir = localHeadLoc - bodyPoints.back();
		bodyPoints.push_back(bodyPoints.back() - newBodyPartDir);
	}
	else
		SpawnNewBodyPartTimer += DeltaTime;

	static LV_COORD LastSavedHeadPoint = HeadImageComponent.GetSceneLocation();
	LastSavedHeadPoint.Align();

	AddMovement(MovingDirection, 1.0f);

	LV_COORD headLocation = HeadImageComponent.GetSceneLocation();
	headLocation.Align();

	if (!LastSavedHeadPoint.Compare(headLocation, 1.0f))
	{
		LV_COORD lastSavedBodyPoint = bodyPoints[0];
		bodyPoints[0] = LastSavedHeadPoint;

		for (size_t i = 1; i < bodyPoints.size(); i++)
		{
			LV_COORD temp = bodyPoints[i];
			bodyPoints[i] = lastSavedBodyPoint;
			lastSavedBodyPoint = temp;
		}

		LastSavedHeadPoint = headLocation;
	}

	BodyImageComponent.SetAbsoluteLocation(bodyPoints[0]);
}

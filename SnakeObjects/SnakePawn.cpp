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
		TailImageComponent.SetImage(imgTail);
		TailImageComponent.SetImageSize({ 1.0f, 1.0f });

		TailImageComponent.SetUseAbsoluteLocation(true);

		AddObjectComponent(&TailImageComponent);
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
	TailImageComponent.SetAbsoluteLocation(tailLocation);
}

void SnakePawn::Update(float DeltaTime)
{
	Inherited::Update(DeltaTime);

	UpdateBodyMovement(DeltaTime);
}

/*
	Input Action bindings.
*/

void SnakePawn::MoveUpDown_Triggered(InputValue actionValue)
{
	// If we already move vertically - don't change direction.
	if (MovingDirection.y != 0.0f) return;

	MovingDirection = { 0.0f, actionValue.AxisValue };
	HandleDirectionChange();
}

void SnakePawn::MoveLeftRight_Triggered(InputValue actionValue)
{
	// If we already move horizontally - don't change direction.
	if (MovingDirection.x != 0.0f) return;

	MovingDirection = { actionValue.AxisValue, 0.0f };
	HandleDirectionChange();
}

void SnakePawn::UpdateBodyMovement(float DeltaTime)
{
	if (MovingDirection == LV_VECTOR::Zero()) return;
	
	AddMovement(MovingDirection, 1.0f);

	LV_COORD tailLocation = TailImageComponent.GetSceneLocation();
	LV_VECTOR tailMovementVec = { 0.0f, 0.0f };

	const float tolerance = 0.08f;
	if (!tailLocation.Compare(TailTargetLocation, tolerance))
	{
		float diff = TailTargetLocation.x - tailLocation.x;
		if (abs(diff) > tolerance)
			tailMovementVec.x = diff > 0.0f ? 1.0f : -1.0f;
		else
		{
			diff = TailTargetLocation.y - tailLocation.y;
			if (abs(diff) > tolerance)
				tailMovementVec.y = diff > 0.0f ? 1.0f : -1.0f;
		}
	}
	else
	{
		if (!turnPoints.empty())
		{
			TailTargetLocation = turnPoints.front();
			turnPoints.pop();
		}
		else TailTargetLocation = HeadImageComponent.GetSceneLocation();
		//else DebugEngineTrap(); // We should never reach parents head with the tail (because it always moves).
	}

	tailLocation += tailMovementVec * MovementPawnComponent.GetMovementSpeed() * DeltaTime;
	TailImageComponent.SetAbsoluteLocation(tailLocation);
}

void SnakePawn::HandleDirectionChange()
{
	/*
	const ImageComponent* prevLastBodyComp = BodyImageComponents.empty() ? nullptr : BodyImageComponents.back().get();

	std::shared_ptr<ImageComponent> newLastBodyComp = std::make_shared<ImageComponent>();
	BodyImageComponents.push(newLastBodyComp);
	
	std::shared_ptr<RCTexture> imgBody(HeadImageComponent.GetImage()->CopyTexture());
	RenderTextureLibrary::FillTextureColor(imgBody.get(), RenderConstants::RedPixelColorRGB);

	newLastBodyComp->SetImage(imgBody);
	newLastBodyComp->SetImageSize({ 1.0f, 1.0f });

	newLastBodyComp->SetUseAbsoluteLocation(true);

	if (prevLastBodyComp)
	{

	}

	AddObjectComponent(newLastBodyComp.get());*/

	turnPoints.push(HeadImageComponent.GetSceneLocation());

	if (TailTargetLocation.IsZero())
	{
		TailTargetLocation = turnPoints.front();
		turnPoints.pop();
	}
}

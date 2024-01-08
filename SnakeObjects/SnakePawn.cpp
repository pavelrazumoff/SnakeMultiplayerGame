#include "SnakePawn.h"

#include "Core/RenderTextureLibrary.h"
#include "Engine/Log/Logger.h"

#include "Engine/SceneObjects/Components/ImageComponent.h"
#include "Engine/SceneObjects/Components/BoxComponent.h"

#include "Food.h"

SnakePawn::SnakePawn()
{
	std::shared_ptr<RCTexture> imgHead = std::make_shared<RCTexture>(1, 1);
	RenderTextureLibrary::FillTexture(imgHead.get(), '*');
	RenderTextureLibrary::FillTextureColor(imgHead.get(), RenderConstants::GreenPixelColorRGB);

	std::shared_ptr<RCTexture> imgTail(imgHead->CopyTexture());
	RenderTextureLibrary::FillTextureColor(imgTail.get(), RenderConstants::BluePixelColorRGB);

	// Head.
	{
		HeadImageComponent = CreateChildComponent<ImageComponent>();

		HeadImageComponent->SetImage(imgHead);
		HeadImageComponent->SetImageSize({ 1.0f, 1.0f });
	}

	// Tail.
	{
		BodyImageComponent = CreateChildComponent<ImageComponent>();

		BodyImageComponent->SetImage(imgTail);
		BodyImageComponent->SetImageSize({ 1.0f, 1.0f });

		BodyImageComponent->SetUseAbsoluteLocation(true);
	}

	// Head Collision Box.
	{
		HeadBoxComponent = CreateChildComponent<BoxComponent>();

		HeadBoxComponent->SetBoxExtent({ 0.5f, 0.5f });

		HeadBoxComponent->OnCollisionStartEvent().Subscribe(this, &SnakePawn::HandleHeadCollisionStartEvent);
		HeadBoxComponent->OnCollisionEndEvent().Subscribe(this, &SnakePawn::HandleHeadCollisionEndEvent);
	}

	// Input Action bindings.
	{
		MoveUpDownAction.Inputs['W'].AxisValue = -1.0f;
		MoveUpDownAction.Inputs['S'].AxisValue = 1.0f;

		InputPawnComponent->BindAction(&MoveUpDownAction, this, &SnakePawn::MoveUpDown_Triggered);

		MoveLeftRightAction.Inputs['A'].AxisValue = -1.0f;
		MoveLeftRightAction.Inputs['D'].AxisValue = 1.0f;

		InputPawnComponent->BindAction(&MoveLeftRightAction, this, &SnakePawn::MoveLeftRight_Triggered);
	}

	if (MovementPawnComponent.Get())
		MovementPawnComponent->SetMovementSpeed({ 15.0f, 10.0f });
}

SnakePawn::~SnakePawn()
{
}

void SnakePawn::BeginPlay()
{
	Inherited::BeginPlay();

	LV_COORD tailLocation = HeadImageComponent->GetSceneLocation() - LV_COORD(0.0f, -1.0f);
	BodyImageComponent->SetAbsoluteLocation(tailLocation);
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

	LV_COORD savedFirstBodyLoc = BodyImageComponent->GetSceneLocation();
	for (int i = 1; i < bodyPoints.size(); ++i)
	{
		BodyImageComponent->SetAbsoluteLocation(bodyPoints[i]);
		BodyImageComponent->DrawComponent(RenderTargetTexture);
	}

	BodyImageComponent->SetAbsoluteLocation(savedFirstBodyLoc);
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

/*
	Snake movement logic.
*/

void SnakePawn::UpdateBodyMovement(float DeltaTime)
{
	if (MovingDirection == LV_VECTOR::Zero()) return;
	
	static LV_COORD LastSavedHeadPoint = HeadImageComponent->GetSceneLocation();
	LastSavedHeadPoint.Round();

	AddMovement(MovingDirection, 1.0f);

	LV_COORD headLocation = HeadImageComponent->GetSceneLocation();
	headLocation.Round();

	if (!LastSavedHeadPoint.Compare(headLocation, 1.0f))
	{
		LV_COORD lastSavedBodyPoint = bodyPoints[0];
		bodyPoints[0] = LastSavedHeadPoint;

		for (size_t i = 1; i < bodyPoints.size(); i++)
		{
			LV_COORD temp = bodyPoints[i];

			// Making sure that every body part is up to date with its predecessor.
			// Because if there will be a huge delay between frames, then the body parts will be out of sync.
			// So we always calc the displacement vector between updated body part and non-updated version of it to get the direction 
			// to move the next one instead of just setting the next one's location to its predecessor old one.
			LV_VECTOR dir = bodyPoints[i - 1] - lastSavedBodyPoint;
			dir.Normalize();

			bodyPoints[i] = bodyPoints[i - 1] - dir;
			lastSavedBodyPoint = temp;
		}

		LastSavedHeadPoint = headLocation;
	}

	BodyImageComponent->SetAbsoluteLocation(bodyPoints[0]);
}

void SnakePawn::IncreaseBody()
{
	LV_COORD localHeadLoc = bodyPoints.size() > 1 ? bodyPoints[bodyPoints.size() - 2] : HeadImageComponent->GetSceneLocation();
	LV_VECTOR newBodyPartDir = localHeadLoc - bodyPoints.back();
	newBodyPartDir.Normalize();

	bodyPoints.push_back(bodyPoints.back() - newBodyPartDir);
}

/*
	Collision Events.
*/

void SnakePawn::HandleHeadCollisionStartEvent(CollisionComponent* InstigatorComp, SceneObject* OtherObject, ICollider* OtherCollider)
{
	if (auto food = dynamic_cast<FoodObject*>(OtherObject))
	{
		food->Destroy();
		IncreaseBody();
	}
}

void SnakePawn::HandleHeadCollisionEndEvent(CollisionComponent* InstigatorComp, SceneObject* OtherObject, ICollider* OtherCollider)
{
}

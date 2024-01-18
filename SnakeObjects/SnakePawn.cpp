#include "SnakePawn.h"

#include "Core/RenderTextureLibrary.h"
#include "Engine/Log/Logger.h"

#include "Engine/SceneObjects/Components/ImageComponent.h"
#include "Engine/SceneObjects/Components/BoxComponent.h"
#include "Components/BodyCollisionComponent.h"

#include "Food.h"

#include "SnakePlayer/SnakePlayerState.h"
#include "Engine/Player/PlayerManager.h"

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

		HeadBoxComponent->SetBoxExtent({ 1.0f, 1.0f });

		HeadBoxComponent->Settings.SetCollisionObjectType(CollisionObjectType::PlayerPawn);
		HeadBoxComponent->Settings.SetCollisionResponse(CollisionObjectType::Static, true);
		HeadBoxComponent->Settings.SetCollisionResponse(CollisionObjectType::Dynamic, true);
		HeadBoxComponent->Settings.SetCollisionResponse(CollisionObjectType::PlayerPawn, true);

		HeadBoxComponent->OnCollisionStartEvent().Subscribe(this, &SnakePawn::HandleHeadCollisionStartEvent);
		HeadBoxComponent->OnCollisionEndEvent().Subscribe(this, &SnakePawn::HandleHeadCollisionEndEvent);
	}

	// Tail Body Collision.
	{
		TailCollisionComponent = CreateChildComponent<BodyCollisionComponent>();

		TailCollisionComponent->Settings.SetCollisionObjectType(CollisionObjectType::PlayerPawn);
		TailCollisionComponent->Settings.SetCollisionResponse(CollisionObjectType::Static, true);
		TailCollisionComponent->Settings.SetCollisionResponse(CollisionObjectType::Dynamic, true);
		TailCollisionComponent->Settings.SetCollisionResponse(CollisionObjectType::PlayerPawn, true);

		TailCollisionComponent->OnCollisionStartEvent().Subscribe(this, &SnakePawn::HandleBodyCollisionStartEvent);
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
	tailLocation.Round();

	BodyImageComponent->SetAbsoluteLocation(tailLocation);

	BODY_POINT tailPoint;
	tailPoint.Location = tailLocation;
	tailPoint.Direction = LV_VECTOR::Zero();

	bodyPoints.push_back(tailPoint);

	AABB lastBodyAABB = { tailPoint.Location.x, tailPoint.Location.y, tailPoint.Location.x, tailPoint.Location.y };
	TailCollisionComponent->AddBodyAABB(lastBodyAABB);
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
		BodyImageComponent->SetAbsoluteLocation(bodyPoints[i].Location);
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
		LV_COORD lastSavedBodyPoint = bodyPoints[0].Location;
		bodyPoints[0].Location = LastSavedHeadPoint;
		bodyPoints[0].Direction = MovingDirection;

		for (size_t i = 1; i < bodyPoints.size(); i++)
		{
			LV_COORD temp = bodyPoints[i].Location;

			// Making sure that every body part is up to date with its predecessor.
			// Because if there will be a huge delay between frames, then the body parts will be out of sync.
			// So we always calc the displacement vector between updated body part and non-updated version of it to get the direction 
			// to move the next one instead of just setting the next one's location to its predecessor old one.
			LV_VECTOR dir = bodyPoints[i - 1].Location - lastSavedBodyPoint;
			dir.Normalize();

			// We need to handle the case when the snake moves out of boundaries and its head is teleported to the other side.
			// In such a case the dir vector will be reversed, so we need to track that and reverse it back.
			bool bChangedDir = false;
			if (bodyPoints[i - 1].Direction.x != 0.0f && dir.x != 0.0f && bodyPoints[i - 1].Direction.x != dir.x)
			{
				dir.x *= -1.0f;
				bChangedDir = true;
			}
			if (bodyPoints[i - 1].Direction.y != 0.0f && dir.y != 0.0f && bodyPoints[i - 1].Direction.y != dir.y)
			{
				dir.y *= -1.0f;
				bChangedDir = true;
			}

			bodyPoints[i].Location = bChangedDir ? lastSavedBodyPoint : (bodyPoints[i - 1].Location - dir);
			bodyPoints[i].Direction = dir;
			lastSavedBodyPoint = temp;
		}

		LastSavedHeadPoint = headLocation;

		SyncBodyMovementWithCollision();
	}

	BodyImageComponent->SetAbsoluteLocation(bodyPoints[0].Location);
}

void SnakePawn::IncreaseBody()
{
	LV_COORD localHeadLoc = (bodyPoints.size() > 1) ? bodyPoints[bodyPoints.size() - 2].Location : HeadImageComponent->GetSceneLocation();
	LV_VECTOR newBodyPartDir = localHeadLoc - bodyPoints.back().Location;
	newBodyPartDir.Normalize();

	BODY_POINT newBodyPart;
	newBodyPart.Location = bodyPoints.back().Location - newBodyPartDir;
	newBodyPart.Direction = newBodyPartDir;

	newBodyPart.Location.Round();
	bodyPoints.push_back(newBodyPart);

	AABB lastBodyAABB = { newBodyPart.Location.x, newBodyPart.Location.y, newBodyPart.Location.x, newBodyPart.Location.y };
	TailCollisionComponent->AddBodyAABB(lastBodyAABB);
}

void SnakePawn::SyncBodyMovementWithCollision()
{
	if (!bodyPoints.size()) return;

	TailCollisionComponent->RemoveBodyAABB(TailCollisionComponent->GetBodyAABBCount() - 1);

	LV_COORD bodyPoint = bodyPoints[0].Location;
	bodyPoint.Round();

	AABB firstBodyAABB = { bodyPoint.x, bodyPoint.y, bodyPoint.x, bodyPoint.y };
	TailCollisionComponent->AddBodyAABB(firstBodyAABB, 0);
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
		UpdatePlayerStat();
	}
	else if (auto snakePawn = dynamic_cast<SnakePawn*>(OtherObject))
	{
		// It means that we collided our head with someone's body (maybe us).

		// TODO: Check that this pawn actually belongs to the current player.
		if (SnakePlayerState* pPlayerState = dynamic_cast<SnakePlayerState*>(PlayerManager::GetInstance().GetPlayerState()))
		{
			pPlayerState->SetPlayerLost();
		}
	}
}

void SnakePawn::HandleHeadCollisionEndEvent(CollisionComponent* InstigatorComp, SceneObject* OtherObject, ICollider* OtherCollider)
{
}

void SnakePawn::HandleBodyCollisionStartEvent(CollisionComponent* InstigatorComp, SceneObject* OtherObject, ICollider* OtherCollider)
{
	
}

void SnakePawn::UpdatePlayerStat()
{
	SnakePlayerState* pPlayerState = dynamic_cast<SnakePlayerState*>(PlayerManager::GetInstance().GetPlayerState());
	if (!pPlayerState) return;

	pPlayerState->IncrementScore();
}

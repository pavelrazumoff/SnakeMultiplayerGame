#pragma once

#include "Engine/SceneObjects/SceneObject.h"
#include "../Components/InputComponent.h"
#include "../Components/MovementComponent.h"

REGISTER_CLASS(GamePawn)
REGISTER_CLASS_FOR_REPLICATION(GamePawn)
class GamePawn : public SceneObject
{
	GAMEOBJECT_BODY(GamePawn, SceneObject)
	ENABLE_REPLICATION(GamePawn)

public:
	GamePawn();
	virtual ~GamePawn();

	virtual void BeginPlay() override;

	virtual void Update(float DeltaTime) override;

	void AddMovement(const LV_VECTOR& Direction, float Scale);

protected:
	void _MovePawn(const LV_VECTOR& ScaledDirection);

protected:
	/** IReplicationObject implementation. */

	virtual void PostReplCreate() override;

	/** Input. */

	virtual void ApplyInputBindings() {}

public:
	/** override EngineGenericType. */

	virtual [[nodiscard]] EngineGenericType* CloneGeneric() const override;
	virtual std::string GetGenericTypeName() const override;

protected:
	void SetControllerNetworkId(uint32_t objId);
	uint32_t GetControllerNetworkId() const { return ControllerNetworkId; }

protected:
	friend class MovementComponent;
	friend class PlayerManager;

	TObjectPtr<InputComponent> InputPawnComponent;
	TObjectPtr<MovementComponent> MovementPawnComponent;

protected:
	uint32_t ControllerNetworkId = 0;
};

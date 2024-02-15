#pragma once

#include "Engine/Player/PlayerState.h"
#include "Engine/Events/EventDelegate.h"

#include <stdint.h>

class SnakePlayerState;

DECLARE_EVENT_DELEGATE(ScoreUpdatedDelegate, SnakePlayerState*, uint32_t);
DECLARE_EVENT_DELEGATE(PlayerLostDelegate, SnakePlayerState*);

class SnakePlayerState : public PlayerState
{
	typedef PlayerState Inherited;

public:
	SnakePlayerState();
	virtual ~SnakePlayerState();

	virtual [[nodiscard]] EngineGenericType* CloneGeneric() const override;
	virtual std::string GetGenericTypeName() const override;

	void IncrementScore();
	void ClearScore();

	uint32_t GetScore() const { return Score; }

	void SetPlayerLost();

	/** Delegates. */

	ScoreUpdatedDelegate& OnScoreUpdatedEvent() { return ScoreUpdatedEvent; }
	PlayerLostDelegate& OnPlayerLostEvent() { return PlayerLostEvent; }

protected:
	/** Delegates. */

	ScoreUpdatedDelegate ScoreUpdatedEvent;
	PlayerLostDelegate PlayerLostEvent;

protected:
	uint32_t Score = 0;
};

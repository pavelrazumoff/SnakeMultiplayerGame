#pragma once

#include "Engine/Player/PlayerState.h"
#include "Engine/Events/EventDelegate.h"

#include <stdint.h>

class SnakePlayerState;

DECLARE_EVENT_DELEGATE(ScoreUpdatedDelegate, SnakePlayerState*, uint32_t);
DECLARE_EVENT_DELEGATE(PlayerEndGameDelegate, SnakePlayerState*, bool);

class SnakePlayerState : public PlayerState
{
	typedef PlayerState Inherited;

public:
	SnakePlayerState();
	virtual ~SnakePlayerState();

	virtual [[nodiscard]] EngineGenericType* Clone() const override;
	virtual std::string GetTypeName() const override;

	void IncrementScore();
	uint32_t GetScore() const { return Score; }

	void SetPlayerWon();
	void SetPlayerLost();

	/** Delegates. */

	ScoreUpdatedDelegate& OnScoreUpdatedEvent() { return ScoreUpdatedEvent; }
	PlayerEndGameDelegate& OnPlayerEndGameEvent() { return PlayerEndGameEvent; }

protected:
	/** Delegates. */

	ScoreUpdatedDelegate ScoreUpdatedEvent;
	PlayerEndGameDelegate PlayerEndGameEvent;

protected:
	uint32_t Score = 0;
};

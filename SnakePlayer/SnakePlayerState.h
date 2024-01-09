#pragma once

#include "Engine/Player/PlayerState.h"
#include "Engine/Events/EventDelegate.h"

#include <stdint.h>

class SnakePlayerState;

DECLARE_EVENT_DELEGATE(ScoreUpdatedDelegate, SnakePlayerState*, uint32_t);

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

	/** Delegates. */

	ScoreUpdatedDelegate& OnScoreUpdatedEvent() { return ScoreUpdatedEvent; }

protected:
	/** Delegates. */

	ScoreUpdatedDelegate ScoreUpdatedEvent;

protected:
	uint32_t Score = 0;
};

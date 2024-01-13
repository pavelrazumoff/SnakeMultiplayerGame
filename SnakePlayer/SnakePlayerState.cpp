#include "SnakePlayerState.h"

SnakePlayerState::SnakePlayerState()
{
}

SnakePlayerState::~SnakePlayerState()
{
}

EngineGenericType* SnakePlayerState::Clone() const
{
	return new SnakePlayerState();
}

std::string SnakePlayerState::GetTypeName() const
{
	return "SnakePlayerState";
}

void SnakePlayerState::IncrementScore()
{
	Score++;
	ScoreUpdatedEvent.Trigger(this, Score);
}

void SnakePlayerState::SetPlayerWon()
{
	PlayerEndGameEvent.Trigger(this, true);
}

void SnakePlayerState::SetPlayerLost()
{
	PlayerEndGameEvent.Trigger(this, false);
}

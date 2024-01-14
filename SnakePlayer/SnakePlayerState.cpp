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

void SnakePlayerState::ClearScore()
{
	Score = 0;
	ScoreUpdatedEvent.Trigger(this, Score);
}

void SnakePlayerState::SetPlayerLost()
{
	PlayerLostEvent.Trigger(this);
}

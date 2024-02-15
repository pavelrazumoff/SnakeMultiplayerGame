#include "SnakePlayerState.h"

#include "Engine/GameObject/GameObjectUtility.h"

SnakePlayerState::SnakePlayerState()
{
}

SnakePlayerState::~SnakePlayerState()
{
}

EngineGenericType* SnakePlayerState::CloneGeneric() const
{
	return CreateNewObject<SnakePlayerState>();
}

std::string SnakePlayerState::GetGenericTypeName() const
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

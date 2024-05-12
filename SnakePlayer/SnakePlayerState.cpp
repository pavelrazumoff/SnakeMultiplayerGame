#include "SnakePlayerState.h"

#include "Engine/GameObject/GameObjectUtility.h"
#include "Engine/Network/ReplicationUtility.h"

void SnakePlayerState::RegisterReplicationMembers()
{
	MAKE_REPLICATED(SnakePlayerState, Score, EPrimitiveType::EPT_Int, &SnakePlayerState::SetPlayerLost);
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
	//ScoreUpdatedEvent.Trigger(this, Score);

	MARK_FOR_REPLICATION(SnakePlayerState, Score);
}

void SnakePlayerState::ClearScore()
{
	Score = 0;
	//ScoreUpdatedEvent.Trigger(this, Score);

	MARK_FOR_REPLICATION(SnakePlayerState, Score);
}

void SnakePlayerState::SetPlayerLost()
{
	PlayerLostEvent.Trigger(this);
}

/*
	Replication.
*/

void SnakePlayerState::OnReplicate_Score()
{
	ScoreUpdatedEvent.Trigger(this, Score);
}

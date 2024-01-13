#pragma once

#include "Engine/Level/GameLevel.h"

class SnakePawn;
class PlayerHUDWidget;
class SnakePlayerState;

class PlayLevel : public GameLevel
{
	typedef GameLevel Inherited;

public:
	PlayLevel();
	virtual ~PlayLevel();

	virtual void OpenLevel() override;
	virtual void Update(float DeltaTime) override;

	virtual void ReconstructLevel() override;

protected:
	void SpawnNewFood();
	void CheckForBoundaries();

	void HandleScoreChanged(SnakePlayerState* Instigator, uint32_t newScore);
	void HandlePlayerEndGame(SnakePlayerState* Instigator, bool bPlayerWon);

protected:
	TObjectPtr<SnakePawn> pSnakePawn;

	TObjectPtr<PlayerHUDWidget> PlayerHUD;

	float SpawnNewFoodTimeValue = 1.5f;
	LV_RECT playAreaRect;

private:
	float SpawnNewFoodTimer = 0.0f;
};
